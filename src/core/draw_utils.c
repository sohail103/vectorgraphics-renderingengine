#include "draw_utils.h"
#include "animations.h"

uint16_t wave[MAX_POINTS * 2];
size_t wave_len = 0;

void add_point(uint8_t x, uint8_t y) {
    if (wave_len < MAX_POINTS - 1) {
        wave[wave_len * 2 + 0] = x << 8;
        wave[wave_len * 2 + 1] = y << 8;
        wave_len++;
    }
}

void draw_line(float x1, float y1, float x2, float y2, int steps) {
    for (int i = 0; i <= steps; i++) {
        float t = (float)i / steps;
        add_point(x1 + t * (x2 - x1), y1 + t * (y2 - y1));
    }
}

void draw_circle(float cx, float cy, float r, int points) {
    for (int i = 0; i <= points; i++) {
        float a = 2 * PI * i / points;
        add_point(cx + r * cosf(a), cy + r * sinf(a));
    }
}

void blank_for(int n) {
    for (int i = 0; i < n; i++) add_point(CX, CY);
}