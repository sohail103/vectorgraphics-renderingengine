#include "animations.h"
#include "draw_utils.h"

void generate_lissajous_frame(float t) {
    wave_len = 0;
    int n = 1500;
    float A = 80.0f;
    float kx = 3.0f, ky = 4.0f, kz = 5.0f;

    for (int i = 0; i < n; i++) {
        float a = 2 * PI * i / n;
        float x = A * sinf(kx * a + t * 0.7f);
        float y = A * sinf(ky * a + t);
        float z = A * sinf(kz * a + t * 0.3f);

        float depth = 1.0f / (z / 150.0f + 2.0f);
        add_point(CX + x * depth, CY + y * depth);
    }
}