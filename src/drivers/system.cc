#include "system.h"
#include "system/system_clock.h"
#include <stm32f3xx_conf.h>

namespace wavesynth {
    void System::Init() {
        ClockConfig();
        system_clock.Init();
    }

    void System::StartTimers() {
        //Enable TIM2 for the matrix
        LL_TIM_EnableCounter(TIM2);
        LL_TIM_GenerateEvent_UPDATE(TIM2);
        //Enable TIM6 for SR timebase
        LL_TIM_EnableCounter(TIM6);
        LL_TIM_GenerateEvent_UPDATE(TIM6);
        //Start systick
        SysTick_Config(72000000 / 1000); 
    }

    void System::ClockConfig() {
        /* Set FLASH latency */
        LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

        /* Enable HSE oscillator */
        LL_RCC_HSE_EnableBypass();
        LL_RCC_HSE_Enable();
        while(LL_RCC_HSE_IsReady() != 1)
        {
        };

        /* Main PLL configuration and activation */
        LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLL_MUL_9, LL_RCC_PREDIV_DIV_1);

        LL_RCC_PLL_Enable();
        while(LL_RCC_PLL_IsReady() != 1)
        {
        };

        /* Sysclk activation on the main PLL */
        LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
        LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
        while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
        {
        };

        /* Set APB1 & APB2 prescaler*/
        LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
        LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
    }

    SystemClock system_clock;
}