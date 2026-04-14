#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NANOSVG_ALL_COLOR_KEYWORDS
#define NANOSVG_IMPLEMENTATION
#include "../lib/nanosvg/nanosvg.h"

// Simple CLI: parse SVG and print shapes in a compact form consumed by svg2c
// Output lines: PATH npoints x1 y1 c1x c1y c2x c2y x2 y2 ...
// CIRCLE cx cy r
// ELLIPSE cx cy rx ry
// RECT x y w h
// LINE x1 y1 x2 y2

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s input.svg\n", argv[0]);
        return 2;
    }
    const char* filename = argv[1];
    NSVGimage* image = nsvgParseFromFile(filename, "px", 96.0f);
    if (!image) {
        fprintf(stderr, "Failed to parse %s\n", filename);
        return 3;
    }

    for (NSVGshape* shape = image->shapes; shape; shape = shape->next) {
        for (NSVGpath* path = shape->paths; path; path = path->next) {
            // path->pts: [x0,y0, x1,y1, ...] where each segment is cubic bezier points
            int npts = path->npts;
            printf("PATH %d", npts / 3);
            for (int i = 0; i < npts * 2; i++) {
                printf(" %.6f", path->pts[i]);
            }
            printf("\n");
        }
    }

    nsvgDelete(image);
    return 0;
}
