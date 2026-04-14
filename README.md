# Vector Graphics Rendering Engine (ESP32)

This project renders vector graphics on a CRT oscilloscope using the ESP32's I2S DACs.

Key points:
- Firmware runs on ESP32 WROOM32 with ESP-IDF via PlatformIO
- Drawing primitives and animations in src/
- Host-side tool `tools/svg2c.py` converts SVG files into C command arrays that the firmware can execute

Quick workflow (host -> device):

1. Convert an SVG to C

   python3 tools/svg2c.py path/to/image.svg -o src/generated_svg.c

   - `svg2c.py` will attempt to use `tools/nanosvg_cli` (more accurate) if present. If not available it will use a conservative built-in parser.

2. Build & Flash (PlatformIO)

   pio run
   pio run -t upload
   pio run -t monitor

   Or using ESP-IDF directly:
   idf.py build
   idf.py flash monitor

3. Switch the active animation mode in `src/main.c` by editing the `ANIMATION_MODE` define.
   - A special mode `MODE_SVG_GENERATED` is available when `generated_svg.c` is added and compiled; set `ANIMATION_MODE` to that mode to render the generated SVG.

Notes & limitations:
- The host-side parser supports path, circle, ellipse, rect, line, polyline, polygon. It supports many common path commands. Arcs and advanced transforms are handled best when `tools/nanosvg_cli` is built and available.
- Large or complex SVGs may need downsampling or chunking to fit the device `MAX_POINTS` (8192).
- After creating `src/generated_svg.c` you must recompile & flash the firmware.

Building the nanosvg helper (optional, recommended):

  ./tools/build_nanosvg_cli.sh

This compiles `tools/nanosvg_cli` which is used by `svg2c.py` for robust parsing.

Example:

  python3 tools/svg2c.py example.svg -o src/generated_svg.c
  pio run -t upload
