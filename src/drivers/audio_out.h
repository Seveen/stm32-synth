#ifndef CONTROLLER_AudioOut_H_
#define CONTROLLER_AudioOut_H_

#include <stdint.h>

namespace wavesynth {
    class AudioOut {
        public:
            AudioOut() {}
            ~AudioOut() {}

            void Init(uint16_t* output_buffer);
            void TransmitSample();
        private:
            uint16_t* buffer;
            void InitGPIO();
            void InitI2S();  
    };
}

#endif
