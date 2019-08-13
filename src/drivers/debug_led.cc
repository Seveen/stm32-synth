#include "debug_led.h"
#include "stm32f3xx.h"
#include "stm32f3xx_conf.h"

namespace wavesynth {
    void DLed::Init() {
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
        LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_OUTPUT);
        LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);
        state = false;
    }

    void DLed::On() {
        LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_5);
        state = true;
    }

    void DLed::Off() {
        LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);
        state = false;
    }

    void DLed::Toggle() {
        if (state) {
            Off();
        } else {
            On();
        }
    }
}
