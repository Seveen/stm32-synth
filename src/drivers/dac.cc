#include "dac.h"

#include <stm32f3xx_conf.h>

namespace wavesynth {
    void Dac::Init(uint16_t* buffer) {
        transmitBuffer = buffer;
        InitDMA();
        InitDAC();
    }

    void Dac::InitDMA() {
        NVIC_SetPriority(DMA2_Channel3_IRQn, 3);
        NVIC_EnableIRQ(DMA2_Channel3_IRQn);
        
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2);
        
        LL_SYSCFG_SetRemapDMA_DAC(LL_SYSCFG_DAC1_CH1_RMP_DMA2_CH3);
        
        LL_DMA_ConfigTransfer(DMA2,
            LL_DMA_CHANNEL_3,
            LL_DMA_DIRECTION_MEMORY_TO_PERIPH |
            LL_DMA_MODE_CIRCULAR              |
            LL_DMA_PERIPH_NOINCREMENT         |
            LL_DMA_MEMORY_NOINCREMENT         |
            LL_DMA_PDATAALIGN_HALFWORD        |
            LL_DMA_MDATAALIGN_HALFWORD        |
            LL_DMA_PRIORITY_HIGH              
        );
        
        LL_DMA_ConfigAddresses(DMA2,
            LL_DMA_CHANNEL_3,
            (uint32_t)transmitBuffer,
            LL_DAC_DMA_GetRegAddr(DAC1, LL_DAC_CHANNEL_1, LL_DAC_DMA_REG_DATA_12BITS_RIGHT_ALIGNED),
            LL_DMA_DIRECTION_MEMORY_TO_PERIPH
        );
        
        LL_DMA_SetDataLength(DMA2,
            LL_DMA_CHANNEL_3,
            1
        );
        
        LL_DMA_EnableIT_TE(DMA2, LL_DMA_CHANNEL_3);
        LL_DMA_EnableChannel(DMA2, LL_DMA_CHANNEL_3);
    }

    void Dac::InitDAC() {
        LL_DAC_InitTypeDef DAC_InitStruct;
        
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA); 
        
        LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_4, LL_GPIO_MODE_ANALOG);
        
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_DAC1);
        
        DAC_InitStruct.TriggerSource            = LL_DAC_TRIG_EXT_TIM6_TRGO;
        DAC_InitStruct.WaveAutoGeneration       = LL_DAC_WAVE_AUTO_GENERATION_NONE;
        DAC_InitStruct.OutputBuffer             = LL_DAC_OUTPUT_BUFFER_ENABLE;
        LL_DAC_Init(DAC1, LL_DAC_CHANNEL_1, &DAC_InitStruct);
        
        LL_DAC_EnableDMAReq(DAC1, LL_DAC_CHANNEL_1);
        
        LL_DAC_EnableIT_DMAUDR1(DAC1);
    }

    void Dac::StartDAC() {
        __IO uint32_t wait_loop_index = 0;

        /* Enable DAC channel */
        LL_DAC_Enable(DAC1, LL_DAC_CHANNEL_1);

        wait_loop_index = ((LL_DAC_DELAY_STARTUP_VOLTAGE_SETTLING_US * (SystemCoreClock / (100000 * 2))) / 10);
        while(wait_loop_index != 0)
        {
            wait_loop_index--;
        }

        LL_DAC_EnableTrigger(DAC1, LL_DAC_CHANNEL_1);
    }
}
