#ifndef CONTROLLER_MODULATOR_H_
#define CONTROLLER_MODULATOR_H_

#include <stdint.h>

#include <stm32f3xx_conf.h>
#include "stm32f3xx.h"

#include "drivers/debug_pin.h"
#include "note_duration.h"

#define SAMPLES_NB_MOD 256.0f
#define SR_MOD 1000.0f

namespace wavesynth {
    class Modulator {
        public:
            Modulator() {}
            ~Modulator() {}

            void Init();
            float Next();
            void SetBeatFrequency(float freq);
            void SetAmplitude(float val);
            void SetOffset(float val);

            void SetWaveToSin();
            void SetWaveToTri();
            void SetWaveToSaw();

            void SetNoteDuration(NoteDuration duration);

            void HardSync();
        private:
            uint16_t* wavetable;

            float phase_acc;
            float beatFrequency;
            float trueFrequency;
            float phase_inc;

            float amplitude;
            float offset;

            float currentNoteDurationFactor;

            void inline ComputeTrueFrequency() {
                trueFrequency = beatFrequency * currentNoteDurationFactor;
            }

            void inline ComputePhaseIncrement() {
                phase_inc = (SAMPLES_NB_MOD/(SR_MOD/trueFrequency));
            }
    };
}

#endif