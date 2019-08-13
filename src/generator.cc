#include "generator.h"
#include "utils/math.h"
#include "wavetables/waves.h"

namespace wavesynth {
    void Generator::Init() {
        phase_acc = 0.0;
        frequency = 1.0;
        phase_inc = 1.0;
        level = 1.0;
        blendLevel = 0.5;
        currentLow = sawtable;
        currentHigh = tritable;
    }

    uint16_t Generator::ProcessNextSample() {
        phase_acc += phase_inc;
        if (phase_acc >= 256.0) {
            phase_acc -= 256.0;
        }
        
        uint8_t int_phase = (uint8_t)phase_acc;
        uint8_t int_phase_next = int_phase + 1;
        float frac = phase_acc - (float)int_phase;

        float result = Math::Lerp(currentLow[int_phase], currentLow[int_phase_next], frac);
        return (uint16_t)((result*level)+0.5f);
    }

    //Prototype for enveloppe. Might be useful when adding xy blending
    uint16_t Generator::ProcessNextBlended() {
        phase_acc += phase_inc;
        if (phase_acc >= 256.0) {
            phase_acc -= 256.0;
        }
        
        uint8_t int_phase = (uint8_t)phase_acc;
        uint8_t int_phase_next = int_phase + 1;
        float frac = phase_acc - (float)int_phase;

        float resultLow = Math::Lerp(currentLow[int_phase], currentLow[int_phase_next], frac);
        float resultHigh = Math::Lerp(currentHigh[int_phase], currentHigh[int_phase_next], frac);

        float result = Math::Lerp(resultLow, resultHigh, blendLevel);

        return (uint16_t)(result*level);
    }

    void Generator::SetFrequency(float freq) {
        frequency = freq;
        ComputePhaseIncrement();
    }

    void Generator::SetLevel(float lvl) {
        this->level = lvl;
    }

    void Generator::HardSync() {
        this->phase_acc = 0.0;
    }

    void Generator::SetBlendLevel(float blend) {
        if (blend < 0.5) {
            currentLow = sawtable;
            currentHigh = tritable;
            blend *= 2;
        } else {
            currentLow = tritable;
            currentHigh = sintable;
            blend -= 0.5;
            blend *= 2;
        }
        this->blendLevel = blend;
    }
}