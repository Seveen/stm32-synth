#ifndef CONTROLLER_ENVELOPE_H_
#define CONTROLLER_ENVELOPE_H_

#include <stdint.h>

#include <stm32f3xx_conf.h>
#include "stm32f3xx.h"

#include "drivers/debug_pin.h"
#include "note_duration.h"

#define SAMPLES_NB_ENV 256.0f
#define SR_ENV 1000.0f

namespace wavesynth {
    enum EnvelopeState {
        ATTACK = 0,
        DECAY = 1,
        STOPPED = 2
    };

    class Envelope {
        public:
            Envelope() {}
            ~Envelope() {}

            void Init();
            float Next();
            void SetBeatFrequency(float freq);
            void SetBlendLevel(float blend);

            void SetNoteDuration(NoteDuration duration);

            void SetAttack(float attack);
            void SetDecay(float decay);

            void HardSync();
            void SetCycling(bool val);
            void ToggleCycling();
            void SetInAttack();
            void SetInDecay();
            void SetFullOpen(bool val);
        private:
            uint16_t* currentLow;
            uint16_t* currentHigh;

            float phase_acc;
            float beatFrequency;
            float trueFrequency;
            float attack_phase_inc;
            float decay_phase_inc;
            float currentNoteDurationFactor;

            float blendLevel;

            float attack;
            float decay;

            bool fullOpen;
            bool cycling;
            EnvelopeState state;

            void inline ComputeTrueFrequency() {
                trueFrequency = beatFrequency * currentNoteDurationFactor;
            }

            void inline ComputeAttackPhaseIncrement() {
                attack_phase_inc = (SAMPLES_NB_ENV/(SR_ENV/(trueFrequency/attack)));
            }

            void inline ComputeDecayPhaseIncrement() {
                decay_phase_inc = (SAMPLES_NB_ENV/(SR_ENV/(trueFrequency/decay)));
            }
    }; 
}

#endif