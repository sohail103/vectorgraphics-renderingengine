#include "animations.h"
#include "i2s_dac.h"
#include "driver/i2s.h"
#include <math.h>

// To render a generated SVG, set ANIMATION_MODE to a special mode that
// calls render_svg() from generated code. Leave as MODE_CAT by default.
#define ANIMATION_MODE   MODE_CAT  // change here

#ifdef __has_include
#if __has_include("generated_svg.c")
extern void render_svg(void);
#define MODE_SVG_GENERATED 99
#endif
#endif

void app_main(void) {
    i2s_dac_init();
    size_t bytes_written;
    float t = 0, ax = 0, ay = 0, blink = 0;

    while (1) {
        switch (ANIMATION_MODE) {
            case MODE_CUBE:       generate_cube_frame(ax, ay); ax+=0.03f; ay+=0.05f; break;
            case MODE_FACE:       generate_face_frame(t, fmodf(blink, 1.0f)); blink+=0.008f; break;
            case MODE_ORBIT:      generate_orbit_frame(t); break;
            case MODE_SPIRO:      generate_spiro_frame(t); break;
            case MODE_TUNNEL:     generate_tunnel_frame(t); break;
            case MODE_DONUT:      generate_donut_frame(t); break;
            case MODE_LISSAJOUS:  generate_lissajous_frame(t); break;
            case MODE_CAT:        generate_cat_frame(t); break;
            case MODE_ELLIPSE:    generate_ellipse_frame(t); break;
            case MODE_BEZIER_DEMO: generate_bezier_demo_frame(t); break;
            case MODE_SVG_GENERATED: render_svg(); break;
        }
        i2s_write(I2S_NUM_0, wave, wave_len * 4, &bytes_written, portMAX_DELAY);
        t += 0.1f;
    }
}
