#include "draw_utils.h"
#include "animations.h"

uint16_t wave[MAX_POINTS * 2];
size_t wave_len = 0;

void add_point(float x, float y) {
    if (wave_len < MAX_POINTS - 1) {
        int ix = (int)(x + 0.5f);
        int iy = (int)(y + 0.5f);
        if (ix < 0) ix = 0;
        if (ix > 255) ix = 255;
        if (iy < 0) iy = 0;
        if (iy > 255) iy = 255;
        wave[wave_len * 2 + 0] = (uint16_t)(ix << 8);
        wave[wave_len * 2 + 1] = (uint16_t)(iy << 8);
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
    draw_ellipse(cx, cy, r, r, points);
}

void draw_ellipse(float cx, float cy, float rx, float ry, int points) {
    for (int i = 0; i <= points; i++) {
        float a = 2 * PI * i / points;
        add_point(cx + rx * cosf(a), cy + ry * sinf(a));
    }
}

void draw_bezier_quadratic(float x0, float y0, float cx, float cy, float x1, float y1, int steps) {
    for (int i = 0; i <= steps; i++) {
        float t = (float)i / steps;
        float mt = 1.0f - t;
        float x = mt * mt * x0 + 2 * mt * t * cx + t * t * x1;
        float y = mt * mt * y0 + 2 * mt * t * cy + t * t * y1;
        add_point(x, y);
    }
}

void draw_bezier_cubic(float x0, float y0, float c1x, float c1y, float c2x, float c2y, float x1, float y1, int steps) {
    for (int i = 0; i <= steps; i++) {
        float t = (float)i / steps;
        float mt = 1.0f - t;
        float x = mt * mt * mt * x0 + 3 * mt * mt * t * c1x + 3 * mt * t * t * c2x + t * t * t * x1;
        float y = mt * mt * mt * y0 + 3 * mt * mt * t * c1y + 3 * mt * t * t * c2y + t * t * t * y1;
        add_point(x, y);
    }
}

void draw_arc(float cx, float cy, float rx, float ry, float start_angle, float end_angle, int points) {
    for (int i = 0; i <= points; i++) {
        float t = (float)i / points;
        float a = start_angle + t * (end_angle - start_angle);
        add_point(cx + rx * cosf(a), cy + ry * sinf(a));
    }
}

void draw_rect(float x, float y, float w, float h, int points_per_side) {
    draw_line(x, y, x + w, y, points_per_side);
    draw_line(x + w, y, x + w, y + h, points_per_side);
    draw_line(x + w, y + h, x, y + h, points_per_side);
    draw_line(x, y + h, x, y, points_per_side);
}

void blank_for(int n) {
    for (int i = 0; i < n; i++) add_point(CX, CY);
}

void reset_frame(void) {
    wave_len = 0;
}