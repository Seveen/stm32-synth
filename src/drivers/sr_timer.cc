#include "sr_timer.h"
#include "stm32f3xx.h"
#include <stm32f3xx_conf.h>

namespace wavesynth {
    void SamplerateTimer::Init() {
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6);
        LL_TIM_SetPrescaler(TIM6, 0);
        LL_TIM_SetAutoReload(TIM6, 72000000/48000);
        LL_TIM_SetCounterMode(TIM6, LL_TIM_COUNTERMODE_DOWN); 
        LL_TIM_SetTriggerOutput(TIM6, LL_TIM_TRGO_UPDATE);
        LL_TIM_EnableIT_UPDATE(TIM6);

        NVIC_SetPriority(TIM6_DAC_IRQn, 1);
        NVIC_EnableIRQ(TIM6_DAC_IRQn);
    }
}