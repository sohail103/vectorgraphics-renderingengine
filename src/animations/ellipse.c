#include "animations.h"
#include "draw_utils.h"

void generate_ellipse_frame(float t) {
    wave_len = 0;
    float rx = 80.0f + 20.0f * sinf(t * 0.7f);
    float ry = 40.0f + 10.0f * cosf(t * 0.9f);
    draw_ellipse(CX, CY, rx, ry, 200);
    blank_for(5);
    // inner rotating ellipse
    draw_ellipse(CX + sinf(t) * 30, CY + cosf(t) * 20, rx/3, ry/3, 120);
}
