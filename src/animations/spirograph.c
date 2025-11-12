#include "animations.h"
#include "draw_utils.h"

void generate_spiro_frame(float t) {
    wave_len = 0;

    float R = 80.0f, r = 35.0f, d = 40.0f;
    int pts = 1000;
    for (int i = 0; i < pts; i++) {
        float a = t + i * 2 * PI / pts;
        float x = (R - r) * cosf(a) + d * cosf((R - r) / r * a);
        float y = (R - r) * sinf(a) - d * sinf((R - r) / r * a);
        add_point(CX + x, CY + y);
    }
}