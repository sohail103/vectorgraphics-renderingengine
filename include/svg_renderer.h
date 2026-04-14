#pragma once

#include <stdint.h>
#include <stddef.h>

typedef enum {
    SVG_CMD_MOVE_TO,
    SVG_CMD_LINE_TO,
    SVG_CMD_LINE,
    SVG_CMD_CIRCLE,
    SVG_CMD_ELLIPSE,
    SVG_CMD_RECT,
    SVG_CMD_BEZIER_QUADRATIC,
    SVG_CMD_BEZIER_CUBIC,
    SVG_CMD_ARC,
    SVG_CMD_BLANK,
    SVG_CMD_END
} SVGCommand;

void svg_render(const SVGCommand* commands, const float* params, size_t num_commands);
void svg_render_single(size_t cmd_index, const SVGCommand* commands, const float* params);
