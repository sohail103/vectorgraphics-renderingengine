#include "animations.h"
#include "draw_utils.h"

void generate_bezier_demo_frame(float t) {
    wave_len = 0;
    // draw a flower using cubic beziers
    for (int i = 0; i < 8; i++) {
        float a = i * (2 * PI / 8) + t * 0.2f;
        float x0 = CX + cosf(a) * 10;
        float y0 = CY + sinf(a) * 10;
        float c1x = CX + cosf(a + 0.3f) * 80;
        float c1y = CY + sinf(a + 0.3f) * 80;
        float c2x = CX + cosf(a - 0.3f) * 80;
        float c2y = CY + sinf(a - 0.3f) * 80;
        float x1 = CX + cosf(a) * 120;
        float y1 = CY + sinf(a) * 120;
        draw_bezier_cubic(x0, y0, c1x, c1y, c2x, c2y, x1, y1, 60);
        blank_for(2);
    }
}
