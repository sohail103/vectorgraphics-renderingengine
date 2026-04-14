#pragma once
#include <stdint.h>

void add_point(float x, float y);
void draw_line(float x1, float y1, float x2, float y2, int steps);
void draw_circle(float cx, float cy, float r, int points);
void draw_ellipse(float cx, float cy, float rx, float ry, int points);
void draw_bezier_quadratic(float x0, float y0, float cx, float cy, float x1, float y1, int steps);
void draw_bezier_cubic(float x0, float y0, float c1x, float c1y, float c2x, float c2y, float x1, float y1, int steps);
void draw_arc(float cx, float cy, float rx, float ry, float start_angle, float end_angle, int points);
void draw_rect(float x, float y, float w, float h, int points_per_side);
void blank_for(int n);
void reset_frame(void);
