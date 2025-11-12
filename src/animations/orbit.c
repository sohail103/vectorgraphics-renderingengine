#include "animations.h"
#include "draw_utils.h"

void generate_orbit_frame(float t) {
    wave_len = 0;
    draw_circle(CX, CY, 10, 60);

    float orbit_r[] = {25, 45, 70, 95};
    float orbit_speed[] = {1.5f, 1.0f, 0.7f, 0.4f};

    for (int p = 0; p < 4; p++) {
        draw_circle(CX, CY, orbit_r[p], 120);
        blank_for(3);

        float a = t * orbit_speed[p];
        float x = CX + orbit_r[p] * cosf(a);
        float y = CY + orbit_r[p] * sinf(a);
        draw_circle(x, y, 4, 40);
        blank_for(3);
    }

    float moonAngle = t * 3.5f;
    float mx = CX + orbit_r[3] * cosf(t * orbit_speed[3]) + 10 * cosf(moonAngle);
    float my = CY + orbit_r[3] * sinf(t * orbit_speed[3]) + 10 * sinf(moonAngle);
    draw_circle(mx, my, 2, 30);
}