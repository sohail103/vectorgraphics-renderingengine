#include "animations.h"
#include "i2s_dac.h"
#include "driver/i2s.h"
#include <math.h>

#define ANIMATION_MODE   MODE_CAT  // change here

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
        }
        i2s_write(I2S_NUM_0, wave, wave_len * 4, &bytes_written, portMAX_DELAY);
        t += 0.1f;
    }
}