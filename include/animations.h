#pragma once

#include <math.h>
#include <stddef.h>
#include <stdint.h>

#define MODE_CUBE        1
#define MODE_FACE        2
#define MODE_ORBIT       3
#define MODE_SPIRO       4
#define MODE_TUNNEL      5
#define MODE_DONUT       6
#define MODE_LISSAJOUS   7
#define MODE_CAT         8

#define MAX_POINTS    8192
#define PI            3.14159265f
#define WIDTH         255
#define HEIGHT        255
#define CX            (WIDTH / 2)
#define CY            (HEIGHT / 2)

extern uint16_t wave[MAX_POINTS * 2];
extern size_t wave_len;

void generate_cube_frame(float ax, float ay);
void generate_face_frame(float mouth, float blink);
void generate_orbit_frame(float t);
void generate_spiro_frame(float t);
void generate_tunnel_frame(float t);
void generate_donut_frame(float t);
void generate_lissajous_frame(float t);
void generate_cat_frame(float t);