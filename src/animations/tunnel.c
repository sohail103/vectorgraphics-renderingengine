#include "animations.h"
#include "draw_utils.h"

void generate_tunnel_frame(float t) {
    wave_len = 0;

    int rings = 12;
    int segments = 200;
    float depth_spacing = 0.18f;
    float twist_speed = 0.8f;
    float wave_amp = 15.0f;

    for (int k = 0; k < rings; k++) {
        float z = (float)k * depth_spacing + fmodf(t * 0.05f, depth_spacing);
        float scale = 1.0f / (z + 0.3f);
        float radius = 40.0f * scale;
        float twist = t * twist_speed + k * 0.4f;

        for (int i = 0; i < segments; i++) {
            float a = 2 * PI * i / segments;
            float wavemod = sinf(5 * a + t) * wave_amp * scale;
            float x = CX + (radius + wavemod) * cosf(a + twist);
            float y = CY + (radius + wavemod) * sinf(a + twist);
            add_point(x, y);
        }
        blank_for(3);
    }
}