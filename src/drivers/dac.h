#ifndef CONTROLLER_DAC_H_
#define CONTROLLER_DAC_H_

#include <stdint.h>

namespace wavesynth {
    class Dac {
        public:
            Dac() {}
            ~Dac() {}

            void Init(uint16_t* buffer);
            void StartDAC();
        private:
            uint16_t* transmitBuffer;

            void InitDMA();
            void InitDAC();          
    };
}

#endif
