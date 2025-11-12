#include "driver/i2s.h"
#include "i2s_dac.h"

void i2s_dac_init(void) {
    i2s_config_t cfg = {
        .mode = I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN,
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S_MSB,
        .intr_alloc_flags = 0,
        .dma_buf_count = 8,
        .dma_buf_len = 256,
        .use_apll = false,
        .tx_desc_auto_clear = true,
        .fixed_mclk = 0
    };
    i2s_driver_install(I2S_NUM_0, &cfg, 0, NULL);
    i2s_set_dac_mode(I2S_DAC_CHANNEL_BOTH_EN);
}