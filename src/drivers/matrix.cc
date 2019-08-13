#include "matrix.h"

#include <stm32f3xx_conf.h>

namespace wavesynth {
    void Matrix::Debounce() {
        for (uint8_t col=0;col<MATRIX_COL;++col) {
            for (uint8_t row=0;row<MATRIX_ROW;++row) {
                uint16_t dataBit = (input_array[col] & mask_array[row]) >> row;
                button_array[col][row] = (button_array[col][row] << 1) | dataBit;
            }
        }
    }

    void Matrix::Init() {
        InitBuffers();
        InitGPIO(); 
        InitDMA();
        InitTIM();
    }

    void Matrix::InitGPIO() {
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);

        WRITE_REG(GPIOB->MODER, 0x55555555);
        WRITE_REG(GPIOB->OTYPER, 0x00000000);
        WRITE_REG(GPIOB->OSPEEDR, 0x00000000);
        WRITE_REG(GPIOB->PUPDR, 0xAAAAAAAA);
        CLEAR_REG(GPIOB->ODR);

        WRITE_REG(GPIOC->MODER, 0x00000000);
        WRITE_REG(GPIOC->PUPDR, 0xAAAAAAAA);
    }

    void Matrix::InitTIM() {
        NVIC_SetPriority(TIM2_IRQn, 4);
        NVIC_EnableIRQ(TIM2_IRQn);
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

        LL_TIM_SetPrescaler(TIM2, __LL_TIM_CALC_PSC(SystemCoreClock, 100000));
        LL_TIM_EnableARRPreload(TIM2);
        LL_TIM_SetAutoReload(TIM2, __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(TIM2), 3000)); //Frequence de rafraichissement
        LL_TIM_OC_SetMode(TIM2, LL_TIM_CHANNEL_CH2, LL_TIM_OCMODE_PWM1);
        LL_TIM_OC_SetCompareCH2(TIM2, ( (LL_TIM_GetAutoReload(TIM2) + 1 ) / 2));
        LL_TIM_OC_EnablePreload(TIM2, LL_TIM_CHANNEL_CH2);

        LL_TIM_SetUpdateSource(TIM2, LL_TIM_UPDATESOURCE_REGULAR);

        LL_TIM_EnableDMAReq_UPDATE(TIM2);
        LL_TIM_EnableDMAReq_CC2(TIM2);

        LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH2);
    }

    void Matrix::InitDMA() {
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

        LL_DMA_ConfigTransfer(DMA1,
            LL_DMA_CHANNEL_2,
            LL_DMA_DIRECTION_MEMORY_TO_PERIPH   |
            LL_DMA_MODE_CIRCULAR                |
            LL_DMA_PERIPH_NOINCREMENT           |
            LL_DMA_MEMORY_INCREMENT             |
            LL_DMA_PDATAALIGN_HALFWORD          |
            LL_DMA_MDATAALIGN_HALFWORD          |
            LL_DMA_PRIORITY_MEDIUM
        );

        LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_2, MATRIX_COL);
        LL_DMA_ConfigAddresses(DMA1, 
            LL_DMA_CHANNEL_2, 
            (uint32_t)&output_array, 
            (uint32_t)&GPIOB->ODR, 
            LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_2)
        );

        LL_DMA_ConfigTransfer(DMA1,
            LL_DMA_CHANNEL_7,
            LL_DMA_DIRECTION_PERIPH_TO_MEMORY   |
            LL_DMA_MODE_CIRCULAR                |
            LL_DMA_PERIPH_NOINCREMENT           |
            LL_DMA_MEMORY_INCREMENT             |
            LL_DMA_PDATAALIGN_HALFWORD          |
            LL_DMA_MDATAALIGN_HALFWORD          |
            LL_DMA_PRIORITY_MEDIUM
        );

        LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_7, MATRIX_COL);
        LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_7, 
            (uint32_t)&GPIOC->IDR, 
            (uint32_t)&input_array, 
            LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_7)
        );

        LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_2);
        LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_7);
    }

    void Matrix::InitBuffers() {
        for (uint8_t col=0;col<MATRIX_COL;col++) {
            for (uint8_t row=0;row<MATRIX_ROW;row++) {
                button_array[col][row] = 0;
            }
        }
    }
}