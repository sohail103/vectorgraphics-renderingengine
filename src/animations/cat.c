#include "animations.h"
#include "draw_utils.h"

void generate_cat_frame(float t) {
    wave_len = 0;

    float baseX = CX - 40;
    float baseY = CY - 20;

    // Body
    for (int i = 0; i <= 200; i++) {
        float a = 2 * PI * i / 200;
        float x = baseX + 60 + 35 * cosf(a);
        float y = baseY + 20 * sinf(a);
        add_point(x, y);
    }
    blank_for(3);

    // Head
    float headX = baseX + 100;
    float headY = baseY + 35;
    for (int i = 0; i <= 100; i++) {
        float a = 2 * PI * i / 100;
        add_point(headX + 15 * cosf(a), headY + 15 * sinf(a));
    }
    blank_for(2);

    // Ears
    draw_line(headX - 10, headY + 15, headX - 20, headY + 30, 20);
    draw_line(headX - 20, headY + 30, headX - 5, headY + 25, 20);
    blank_for(2);
    draw_line(headX + 10, headY + 15, headX + 20, headY + 30, 20);
    draw_line(headX + 20, headY + 30, headX + 5, headY + 25, 20);
    blank_for(3);

    // Tail (to the right)
    float tailBaseX = baseX + 100;
    float tailBaseY = baseY + 25;
    for (int i = 0; i < 80; i++) {
        float s = (float)i / 80;
        float x = tailBaseX + s * 60;
        float y = tailBaseY + sinf(s * 3 * PI + t * 1.5f) * 15.0f * (1.0f - s);
        add_point(x, y);
    }
    blank_for(3);

    // Legs
    float legAmp = 10.0f;
    float legPhase = t * 2.0f;
    draw_line(baseX + 80, baseY, baseX + 80 + sinf(legPhase) * legAmp, baseY - 25, 20);
    draw_line(baseX + 95, baseY, baseX + 95 - sinf(legPhase) * legAmp, baseY - 25, 20);
    draw_line(baseX + 30, baseY, baseX + 30 - sinf(legPhase) * legAmp, baseY - 25, 20);
    draw_line(baseX + 45, baseY, baseX + 45 + sinf(legPhase) * legAmp, baseY - 25, 20);
}