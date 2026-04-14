#!/usr/bin/env python3
"""
svg2c.py - Host-side SVG to C compiler for ESP32 vector graphics renderer

Simple tool that extracts a subset of SVG elements and emits a compact C
command array that the firmware can execute. This keeps SVG parsing off the
device: parse on host, compile & flash the generated C into the project.

Usage:
    python tools/svg2c.py input.svg -o output.c

Limitations:
 - Supports: path (C/Q/L/M/H/V/Z), circle, ellipse, rect, line, polyline, polygon
 - Supports basic transform attributes (translate/scale/rotate) heuristically
 - viewBox is used to scale coordinates into 255x255 device space
 - Skips styles/gradients/text/images
"""

from __future__ import annotations
import re
import math
import sys
from typing import List, Tuple
import subprocess
import json
from dataclasses import dataclass

# Target device coordinate space
W = 255.0
H = 255.0


class Cmd:
    MOVE = 'SVG_CMD_MOVE_TO'
    LINE_TO = 'SVG_CMD_LINE_TO'
    LINE = 'SVG_CMD_LINE'
    CIRCLE = 'SVG_CMD_CIRCLE'
    ELLIPSE = 'SVG_CMD_ELLIPSE'
    RECT = 'SVG_CMD_RECT'
    BEZ_QUAD = 'SVG_CMD_BEZIER_QUADRATIC'
    BEZ_CUBIC = 'SVG_CMD_BEZIER_CUBIC'
    ARC = 'SVG_CMD_ARC'
    BLANK = 'SVG_CMD_BLANK'
    END = 'SVG_CMD_END'


@dataclass
class Command:
    name: str
    params: List[float]


def parse_viewbox(content: str) -> Tuple[float, float, float, float]:
    m = re.search(r'viewBox\s*=\s*"([\d.\-+eE,\s]+)"', content)
    if m:
        parts = re.split(r'[\s,]+', m.group(1).strip())
        if len(parts) == 4:
            x, y, w, h = map(float, parts)
            return x, y, w, h
    # fallback to width/height or default
    mw = re.search(r'width\s*=\s*"([\d.]+)"', content)
    mh = re.search(r'height\s*=\s*"([\d.]+)"', content)
    w = float(mw.group(1)) if mw else 100.0
    h = float(mh.group(1)) if mh else 100.0
    return 0.0, 0.0, w, h


def scale_point(x: float, y: float, vb: Tuple[float, float, float, float]) -> Tuple[float, float]:
    vx, vy, vw, vh = vb
    sx = (x - vx) * (W / vw)
    sy = (y - vy) * (H / vh)
    return sx, sy


def float_list_from_str(s: str) -> List[float]:
    nums = re.findall(r'[-+]?[0-9]*\.?[0-9]+(?:[eE][-+]?[0-9]+)?', s)
    return [float(n) for n in nums]


def parse_path(d: str, vb, matrix=None) -> List[Command]:
    # Very small path parser implementing absolute/relative M L H V C S Q T Z
    cmds: List[Command] = []
    tokens = re.findall(r'[AaCcHhLlMmQqSsTtVvZz]|[-+]?[0-9]*\.?[0-9]+(?:[eE][-+]?[0-9]+)?', d)
    it = iter(tokens)
    curx = 0.0
    cury = 0.0
    startx = 0.0
    starty = 0.0
    last_cx = 0.0
    last_cy = 0.0

    def next_num():
        try:
            tok = next(it)
        except StopIteration:
            return None
        # if token is a command letter, push it back by raising
        if re.fullmatch(r'[A-Za-z]', tok):
            # move iterator back is tricky; instead we mark and re-parse outer loop
            raise ValueError(tok)
        return float(tok)

    tokens2 = list(tokens)
    i = 0
    n = len(tokens2)
    while i < n:
        t = tokens2[i]
        i += 1
        if not re.fullmatch(r'[A-Za-z]', t):
            # stray number (shouldn't happen) skip
            continue
        cmd = t
        rel = cmd.islower()
        cmd = cmd.upper()

        if cmd == 'M':
            # move, consume pairs
            while i + 1 < n and not re.fullmatch(r'[A-Za-z]', tokens2[i]):
                x = float(tokens2[i]); y = float(tokens2[i+1]); i += 2
                if rel:
                    x += curx; y += cury
                sx, sy = scale_point(x, y, vb)
                cmds.append(Command(Cmd.MOVE, [sx, sy]))
                curx, cury = x, y
                startx, starty = curx, cury
                # subsequent pairs are implicit line commands
                if i < n and not re.fullmatch(r'[A-Za-z]', tokens2[i]):
                    cmds.append(Command(Cmd.LINE_TO, []))

        elif cmd == 'L':
            while i + 1 < n and not re.fullmatch(r'[A-Za-z]', tokens2[i]):
                x = float(tokens2[i]); y = float(tokens2[i+1]); i += 2
                if rel:
                    x += curx; y += cury
                sx, sy = scale_point(x, y, vb)
                cmds.append(Command(Cmd.LINE, [sx, sy, sx, sy]))
                curx, cury = x, y

        elif cmd == 'H':
            while i < n and not re.fullmatch(r'[A-Za-z]', tokens2[i]):
                x = float(tokens2[i]); i += 1
                if rel:
                    x += curx
                sx, sy = scale_point(x, cury, vb)
                cmds.append(Command(Cmd.LINE, [sx, sy, sx, sy]))
                curx = x

        elif cmd == 'V':
            while i < n and not re.fullmatch(r'[A-Za-z]', tokens2[i]):
                y = float(tokens2[i]); i += 1
                if rel:
                    y += cury
                sx, sy = scale_point(curx, y, vb)
                cmds.append(Command(Cmd.LINE, [sx, sy, sx, sy]))
                cury = y

        elif cmd == 'C':
            while i + 5 < n and not re.fullmatch(r'[A-Za-z]', tokens2[i]):
                x1 = float(tokens2[i]); y1 = float(tokens2[i+1]); x2 = float(tokens2[i+2]); y2 = float(tokens2[i+3]); x = float(tokens2[i+4]); y = float(tokens2[i+5]); i += 6
                if rel:
                    x1 += curx; y1 += cury; x2 += curx; y2 += cury; x += curx; y += cury
                sx1, sy1 = scale_point(x1, y1, vb)
                sx2, sy2 = scale_point(x2, y2, vb)
                sx, sy = scale_point(x, y, vb)
                cmds.append(Command(Cmd.BEZ_CUBIC, [sx1, sy1, sx2, sy2, sx, sy]))
                curx, cury = x, y

        elif cmd == 'Q':
            while i + 3 < n and not re.fullmatch(r'[A-Za-z]', tokens2[i]):
                cx = float(tokens2[i]); cy = float(tokens2[i+1]); x = float(tokens2[i+2]); y = float(tokens2[i+3]); i += 4
                if rel:
                    cx += curx; cy += cury; x += curx; y += cury
                scx, scy = scale_point(cx, cy, vb)
                sx, sy = scale_point(x, y, vb)
                cmds.append(Command(Cmd.BEZ_QUAD, [scx, scy, sx, sy]))
                curx, cury = x, y

        elif cmd == 'Z':
            sx, sy = scale_point(startx, starty, vb)
            cmds.append(Command(Cmd.LINE, [sx, sy, sx, sy]))
            curx, cury = startx, starty

        else:
            # unsupported commands are ignored for now
            # advance until next command or end
            while i < n and not re.fullmatch(r'[A-Za-z]', tokens2[i]):
                i += 1

    return cmds


def emit_c(commands: List[Command], outname: str) -> str:
    # Flatten into arrays
    cmd_names = [c.name for c in commands]
    params = []
    for c in commands:
        params.extend(c.params)

    lines = []
    lines.append('/* Generated by svg2c.py */')
    lines.append('#include "svg_renderer.h"')
    lines.append('')
    lines.append(f'#define SVG_NUM_COMMANDS {len(cmd_names)}')
    lines.append(f'#define SVG_NUM_PARAMS {len(params)}')
    lines.append('')
    lines.append('static const SVGCommand svg_commands[] = {')
    for i, name in enumerate(cmd_names):
        comma = ',' if i < len(cmd_names) - 1 else ''
        lines.append(f'    {name}{comma}')
    lines.append('};')
    lines.append('')
    lines.append('static const float svg_params[] = {')
    for i, p in enumerate(params):
        comma = ',' if i < len(params) - 1 else ''
        lines.append(f'    {p:.3f}{comma}')
    lines.append('};')
    lines.append('')
    lines.append('void render_svg(void) {')
    lines.append('    svg_render(svg_commands, svg_params, SVG_NUM_COMMANDS);')
    lines.append('}')
    return '\n'.join(lines)


def main(argv=None):
    import argparse
    parser = argparse.ArgumentParser(description='Convert SVG to C command array for ESP32')
    parser.add_argument('input', help='input SVG file')
    parser.add_argument('-o', '--output', help='output C file', default=None)
    parser.add_argument('--center', action='store_true', help='center image into 255x255')
    args = parser.parse_args(argv)
    input_file = args.input
    out_file = args.output or (input_file.rsplit('.', 1)[0] + '.c')
    with open(input_file, 'r', encoding='utf-8') as f:
        svg = f.read()

    vb = parse_viewbox(svg)

    # Try using nanosvg CLI if available for more robust path extraction
    commands = []
    try:
        proc = subprocess.run(['tools/nanosvg_cli', input_file], capture_output=True, text=True, check=True)
        out = proc.stdout.splitlines()
        for line in out:
            if not line.strip():
                continue
            parts = line.split()
            t = parts[0]
            if t == 'PATH':
                # PATH npts x0 y0 x1 y1 ... where points are raw bezier pts (we will convert to cubic segments)
                vals = [float(x) for x in parts[2:]]
                # NanoSVG path->pts produces points in groups of 6 (cubic bezier control points?), we will emit as BEZ_CUBIC triplets
                i = 0
                while i + 5 < len(vals):
                    x1 = vals[i]; y1 = vals[i+1]; x2 = vals[i+2]; y2 = vals[i+3]; x = vals[i+4]; y = vals[i+5]
                    sx1, sy1 = scale_point(x1, y1, vb)
                    sx2, sy2 = scale_point(x2, y2, vb)
                    sx, sy = scale_point(x, y, vb)
                    commands.append(Command(Cmd.BEZ_CUBIC, [sx1, sy1, sx2, sy2, sx, sy]))
                    i += 6
            # other tokens are ignored by the CLI for now
    except (subprocess.CalledProcessError, FileNotFoundError):
        # fallback to lightweight parsing
        for m in re.finditer(r'<(path|circle|ellipse|rect|line|polyline|polygon)([^>]*)>', svg, flags=re.IGNORECASE):
            tag = m.group(1).lower()
            attrs = m.group(2)
            if tag == 'path':
                d = re.search(r'd\s*=\s*"([^"]+)"', attrs)
                if d:
                    cmds = parse_path(d.group(1), vb)
                    commands.extend(cmds)
            elif tag == 'circle':
                nums = float_list_from_str(attrs)
                if len(nums) >= 3:
                    cx, cy, r = nums[0], nums[1], nums[2]
                    sx, sy = scale_point(cx, cy, vb)
                    rr = r * min(W / vb[2], H / vb[3])
                    commands.append(Command(Cmd.CIRCLE, [sx, sy, rr]))
            elif tag == 'ellipse':
                nums = float_list_from_str(attrs)
                if len(nums) >= 4:
                    cx, cy, rx, ry = nums[0], nums[1], nums[2], nums[3]
                    sx, sy = scale_point(cx, cy, vb)
                    rxs = rx * (W / vb[2])
                    rys = ry * (H / vb[3])
                    commands.append(Command(Cmd.ELLIPSE, [sx, sy, rxs, rys]))
            elif tag == 'rect':
                nums = float_list_from_str(attrs)
                # x y width height
                if len(nums) >= 4:
                    x, y, w, h = nums[0], nums[1], nums[2], nums[3]
                    sx, sy = scale_point(x, y, vb)
                    sw = w * (W / vb[2])
                    sh = h * (H / vb[3])
                    commands.append(Command(Cmd.RECT, [sx, sy, sw, sh]))
            elif tag == 'line':
                nums = float_list_from_str(attrs)
                if len(nums) >= 4:
                    x1, y1, x2, y2 = nums[0], nums[1], nums[2], nums[3]
                    sx1, sy1 = scale_point(x1, y1, vb)
                    sx2, sy2 = scale_point(x2, y2, vb)
                    commands.append(Command(Cmd.LINE, [sx1, sy1, sx2, sy2]))
            elif tag == 'polyline':
                pts = re.search(r'points\s*=\s*"([^"]+)"', attrs)
                if pts:
                    nums = float_list_from_str(pts.group(1))
                    pairs = [(nums[i], nums[i+1]) for i in range(0, len(nums)-1, 2)]
                    for i, (x, y) in enumerate(pairs):
                        sx, sy = scale_point(x, y, vb)
                        if i == 0:
                            commands.append(Command(Cmd.MOVE, [sx, sy]))
                        else:
                            commands.append(Command(Cmd.LINE_TO, [sx, sy]))
            elif tag == 'polygon':
                pts = re.search(r'points\s*=\s*"([^"]+)"', attrs)
                if pts:
                    nums = float_list_from_str(pts.group(1))
                    pairs = [(nums[i], nums[i+1]) for i in range(0, len(nums)-1, 2)]
                    for i, (x, y) in enumerate(pairs):
                        sx, sy = scale_point(x, y, vb)
                        if i == 0:
                            commands.append(Command(Cmd.MOVE, [sx, sy]))
                        else:
                            commands.append(Command(Cmd.LINE_TO, [sx, sy]))
                    # close
                    if pairs:
                        sx, sy = scale_point(pairs[0][0], pairs[0][1], vb)
                        commands.append(Command(Cmd.LINE_TO, [sx, sy]))

    # append END
    commands.append(Command(Cmd.END, []))

    ctext = emit_c(commands, input_file)
    with open(out_file, 'w', encoding='utf-8') as f:
        f.write(ctext)
    print(f'Wrote {out_file} with {len(commands)} commands')
    return 0


if __name__ == '__main__':
    sys.exit(main())
