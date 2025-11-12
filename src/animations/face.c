#include "animations.h"
#include "draw_utils.h"

static void draw_eye(float cx, float cy, float r, float blink) {
    int pts = 100;
    for (int i = 0; i <= pts; i++) {
        float a = PI * i / pts;
        float y = sinf(a) * (1.0f - blink);
        add_point(cx + r * cosf(a), cy + r * y);
    }
}

void generate_face_frame(float mouthPhase, float blinkPhase) {
    wave_len = 0;

    draw_circle(CX, CY, 90, 400);
    blank_for(3);

    float blink = (blinkPhase > 0.95f) ? 1.0f - (blinkPhase - 0.95f) * 20.0f : 0.0f;
    if (blinkPhase > 0.975f) blink = 0.0f;

    draw_eye(CX - 35, CY + 30, 10, blink);
    blank_for(2);
    draw_eye(CX + 35, CY + 30, 10, blink);
    blank_for(2);

    float open = (sinf(mouthPhase) * 0.5f + 0.5f) * 25.0f + 5.0f;
    int pts = 120;
    for (int i = 0; i <= pts; i++) {
        float a = PI * i / pts;
        float x = CX - 50 + 100.0f * i / pts;
        float y = CY - 40 - sinf(a) * open;
        add_point(x, y);
    }
}