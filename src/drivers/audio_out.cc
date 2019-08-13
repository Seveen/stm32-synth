#include <audio_out.h>

#include <stm32f3xx_conf.h>

namespace wavesynth {
    void AudioOut::Init(uint16_t* output_buffer) {
        buffer = output_buffer;
        InitGPIO();
        InitI2S();
    }

    void AudioOut::TransmitSample() {
        LL_I2S_TransmitData16(SPI2, *buffer);
    }

    void AudioOut::InitGPIO() {
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

        LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_11, LL_GPIO_MODE_ALTERNATE);
        LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_12, LL_GPIO_MODE_ALTERNATE);
        LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_13, LL_GPIO_MODE_ALTERNATE);

        LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_11, LL_GPIO_AF_5);
        LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_12, LL_GPIO_AF_5);
        LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_13, LL_GPIO_AF_5);
    }

    void AudioOut::InitI2S() {
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);

        LL_I2S_InitTypeDef i2s_init;
        LL_I2S_StructInit(&i2s_init);

        i2s_init.AudioFreq = LL_I2S_AUDIOFREQ_48K;
        i2s_init.Mode = LL_I2S_MODE_MASTER_TX;
        i2s_init.ClockPolarity = LL_I2S_POLARITY_LOW;
        i2s_init.DataFormat = LL_I2S_DATAFORMAT_16B;
        i2s_init.MCLKOutput = LL_I2S_MCLK_OUTPUT_DISABLE;
        i2s_init.Standard = LL_I2S_STANDARD_PHILIPS;
        LL_I2S_Init(SPI2, &i2s_init);

        LL_I2S_Enable(SPI2);
    }
}
