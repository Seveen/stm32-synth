#ifndef CONTROLLER_GENERATOR_H_
#define CONTROLLER_GENERATOR_H_

#include <stdint.h>

#include <stm32f3xx_conf.h>
#include "stm32f3xx.h"

#include "drivers/debug_pin.h"

#define SAMPLES_NB_GEN 256.0f
#define SR_GEN 48000.0f

namespace wavesynth {
    class Generator {
        public:
            Generator() {}
            ~Generator() {}

            void Init();
            uint16_t ProcessNextSample();
            uint16_t ProcessNextBlended();
            void SetFrequency(float freq);
            void SetLevel(float lvl);
            void SetBlendLevel(float blend);

            void HardSync();
        private:
            uint16_t* currentLow;
            uint16_t* currentHigh;

            float phase_acc;
            float frequency;
            float phase_inc;

            float level;

            float blendLevel;

            void inline ComputePhaseIncrement() {
                phase_inc = (SAMPLES_NB_GEN/(SR_GEN/frequency));
            }
    };
}

#endif