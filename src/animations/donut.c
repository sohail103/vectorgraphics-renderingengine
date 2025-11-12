#include "animations.h"
#include "draw_utils.h"

void generate_donut_frame(float t) {
    wave_len = 0;

    float R = 60.0f, r = 20.0f;
    int nTheta = 60, nPhi = 30;
    float sinA = sinf(t * 0.6f), cosA = cosf(t * 0.6f);
    float sinB = sinf(t * 0.4f), cosB = cosf(t * 0.4f);

    for (int i = 0; i < nTheta; i++) {
        float theta = 2 * PI * i / nTheta;
        for (int j = 0; j < nPhi; j++) {
            float phi = 2 * PI * j / nPhi;

            float x = (R + r * cosf(phi)) * cosf(theta);
            float y = (R + r * cosf(phi)) * sinf(theta);
            float z = r * sinf(phi);

            float x1 = x * cosA - y * sinA;
            float y1 = x * sinA + y * cosA;
            float y2 = y1 * cosB - z * sinB;
            float z2 = y1 * sinB + z * cosB;

            float depth = 1.0f / (z2 * 0.02f + 2.0f);
            add_point(CX + x1 * depth, CY + y2 * depth);
        }
        blank_for(2);
    }
}