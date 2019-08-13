#include "adc.h"

#include <stm32f3xx_conf.h>
#include <stm32f3xx.h>

namespace wavesynth {
    uint16_t Adc::GetChannelValue(uint8_t channel) {
        return adcBuffer[channel];
    }

    float Adc::GetNormalizedChannelValue(uint8_t channel) {
        //ADC 12bits => 4096
        return (float)adcBuffer[channel]/4096.0;
    }

    void Adc::Init() {
        InitBuffer();
        InitDMA();
        InitGPIO();
        InitADC();
    }

    void Adc::InitBuffer() {
        for (uint8_t i=0;i<5;i++) {
            adcBuffer[i] = 0;
        }
    }

    void Adc::InitDMA() {
        NVIC_SetPriority(DMA1_Channel1_IRQn, 5);  /* DMA IRQ lower priority than ADC IRQ */
        NVIC_EnableIRQ(DMA1_Channel1_IRQn);

        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

        LL_DMA_ConfigTransfer(DMA1,
            LL_DMA_CHANNEL_1,
            LL_DMA_DIRECTION_PERIPH_TO_MEMORY |
            LL_DMA_MODE_CIRCULAR              |
            LL_DMA_PERIPH_NOINCREMENT         |
            LL_DMA_MEMORY_INCREMENT           |
            LL_DMA_PDATAALIGN_HALFWORD        |
            LL_DMA_MDATAALIGN_HALFWORD        |
            LL_DMA_PRIORITY_HIGH               
        );

        LL_DMA_ConfigAddresses(DMA1,
            LL_DMA_CHANNEL_1,
            LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA),
            (uint32_t)&adcBuffer,
            LL_DMA_DIRECTION_PERIPH_TO_MEMORY
        );

        LL_DMA_SetDataLength(DMA1,
            LL_DMA_CHANNEL_1,
            5
        );
        
        LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);
        LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_1);
        LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
    }

    void Adc::InitGPIO() {
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
        
        LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_ANALOG);
        LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_1, LL_GPIO_MODE_ANALOG);
        LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_ANALOG);
        LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_3, LL_GPIO_MODE_ANALOG);        
        LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_11, LL_GPIO_MODE_ANALOG);
    }

    void Adc::InitADC() {
        LL_ADC_SetCommonClock(__LL_ADC_COMMON_INSTANCE(ADC2), LL_ADC_CLOCK_SYNC_PCLK_DIV2);
        NVIC_SetPriority(ADC1_2_IRQn, 6);
        NVIC_EnableIRQ(ADC1_2_IRQn);  
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_ADC12);

        LL_ADC_SetCommonClock(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_CLOCK_SYNC_PCLK_DIV2);
        LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), (LL_ADC_PATH_INTERNAL_VREFINT | LL_ADC_PATH_INTERNAL_TEMPSENSOR));
        uint32_t wait_loop_index = ((LL_ADC_DELAY_TEMPSENSOR_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
        while(wait_loop_index != 0)
        {
            wait_loop_index--;
        }

        if ((LL_ADC_IsEnabled(ADC1) == 0) ||
            (LL_ADC_REG_IsConversionOngoing(ADC1) == 0)) {

            LL_ADC_REG_SetTriggerSource(ADC1, LL_ADC_REG_TRIG_SOFTWARE);
            LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_CONTINUOUS);
            LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);
            LL_ADC_REG_SetOverrun(ADC1, LL_ADC_REG_OVR_DATA_OVERWRITTEN);
            LL_ADC_REG_SetSequencerLength(ADC1, LL_ADC_REG_SEQ_SCAN_ENABLE_5RANKS);

            /* Set ADC group regular sequence: channel on the selected sequence rank. */
            LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_1);
            LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_2, LL_ADC_CHANNEL_2);
            LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_3, LL_ADC_CHANNEL_3);
            LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_4, LL_ADC_CHANNEL_4);
            LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_5, LL_ADC_CHANNEL_14);

            LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_1, LL_ADC_SAMPLINGTIME_19CYCLES_5);
            LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_2, LL_ADC_SAMPLINGTIME_19CYCLES_5);
            LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_3, LL_ADC_SAMPLINGTIME_19CYCLES_5);
            LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_4, LL_ADC_SAMPLINGTIME_19CYCLES_5);
            LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_14, LL_ADC_SAMPLINGTIME_19CYCLES_5);

            LL_ADC_EnableIT_EOS(ADC1);
            LL_ADC_EnableIT_OVR(ADC1);
        }
    }

    void Adc::StartADC() {
        __IO uint32_t wait_loop_index = 0;

        if (LL_ADC_IsEnabled(ADC1) == 0)
        {
            LL_ADC_EnableInternalRegulator(ADC1);

            wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
            while(wait_loop_index != 0)
            {
                wait_loop_index--;
            }
            
            LL_ADC_StartCalibration(ADC1, LL_ADC_SINGLE_ENDED);
            
            while (LL_ADC_IsCalibrationOnGoing(ADC1) != 0) {
            }
            
            wait_loop_index = (LL_ADC_DELAY_CALIB_ENABLE_ADC_CYCLES * 32 >> 1);
            while(wait_loop_index != 0)
            {
                wait_loop_index--;
            }
            
            LL_ADC_Enable(ADC1);
            
            while (LL_ADC_IsActiveFlag_ADRDY(ADC1) == 0)
            {
            }
            LL_ADC_REG_StartConversion(ADC1);
        }
    }
} 

extern "C" {
    void DMA1_Channel1_IRQHandler(void) {
	
		if(LL_DMA_IsActiveFlag_TC1(DMA1) == 1)
		{
			LL_DMA_ClearFlag_GI1(DMA1);
		}
		
		if(LL_DMA_IsActiveFlag_TE1(DMA1) == 1)
		{
			LL_DMA_ClearFlag_TE1(DMA1);
		}
	}

    void ADC1_2_IRQHandler(void) {
		if(LL_ADC_IsActiveFlag_EOS(ADC1) != 0) {
			LL_ADC_ClearFlag_EOS(ADC1);
		}
		
		if(LL_ADC_IsActiveFlag_OVR(ADC1) != 0) {
			LL_ADC_ClearFlag_OVR(ADC1);
		}
	}
}