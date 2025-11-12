#include "animations.h"
#include "draw_utils.h"

typedef struct { float x, y, z; } Vec3;

static const Vec3 cube_vertices[8] = {
    {-1,-1,-1},{1,-1,-1},{1,1,-1},{-1,1,-1},
    {-1,-1,1},{1,-1,1},{1,1,1},{-1,1,1}
};

static const int cube_edges[12][2] = {
    {0,1},{1,2},{2,3},{3,0},
    {4,5},{5,6},{6,7},{7,4},
    {0,4},{1,5},{2,6},{3,7}
};

void generate_cube_frame(float ax, float ay) {
    wave_len = 0;
    float scale = 80.0f, perspective = 2.0f;
    float sinX = sinf(ax), cosX = cosf(ax);
    float sinY = sinf(ay), cosY = cosf(ay);

    Vec3 p[8];
    for (int i = 0; i < 8; i++) {
        Vec3 v = cube_vertices[i];
        float x1 = v.x * cosY + v.z * sinY;
        float z1 = -v.x * sinY + v.z * cosY;
        float y2 = v.y * cosX - z1 * sinX;
        float z2 = v.y * sinX + z1 * cosX;
        float depth = 1.0f / (z2 + perspective);
        p[i].x = CX + scale * x1 * depth;
        p[i].y = CY + scale * y2 * depth;
    }

    for (int e = 0; e < 12; e++) {
        draw_line(p[cube_edges[e][0]].x, p[cube_edges[e][0]].y,
                  p[cube_edges[e][1]].x, p[cube_edges[e][1]].y, 40);
        blank_for(2);
    }
}
