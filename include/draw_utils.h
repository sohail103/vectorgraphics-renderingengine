#pragma once
#include <stdint.h>

void add_point(uint8_t x, uint8_t y);
void draw_line(float x1, float y1, float x2, float y2, int steps);
void draw_circle(float cx, float cy, float r, int points);
void blank_for(int n);