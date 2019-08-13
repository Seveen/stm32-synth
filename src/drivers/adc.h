#ifndef CONTROLLER_ADC_H_
#define CONTROLLER_ADC_H_

#include <stdint.h>

#define ADC_NB 5

namespace wavesynth {
    class Adc {
        public:
            Adc() {}
            ~Adc() {}

            void Init();
            void StartADC();
            uint16_t GetChannelValue(uint8_t channel);
            float GetNormalizedChannelValue(uint8_t channel);
        private:
            uint16_t adcBuffer[ADC_NB];
            void InitBuffer();
            void InitDMA();
            void InitGPIO();
            void InitADC();
    };
}

#endif