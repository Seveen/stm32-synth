#include "modulator.h"
#include "wavetables/waves.h"
#include "utils/math.h"

namespace wavesynth {
    void Modulator::Init() {
        phase_acc = 0.0;
        amplitude = 1.0;
        offset = 0.0;
        beatFrequency = 1.0;
        currentNoteDurationFactor = 1.0;
        ComputeTrueFrequency();
        ComputePhaseIncrement();
        wavetable = sintable;
    }

    float Modulator::Next() {
        phase_acc += phase_inc;
        if (phase_acc >= 256.0) {
            phase_acc -= 256.0;
        }
        
        uint8_t int_phase = (uint8_t)phase_acc;
        uint8_t int_phase_next = int_phase + 1;
        float frac = phase_acc - (float)int_phase;

        float result = Math::Lerp(wavetable[int_phase], wavetable[int_phase_next], frac);
        result /= 4096.0;
        result *= amplitude;
        result += offset;
        if (result > 1.0) {
            result = 1.0;
        }
        return result;
    }

    void Modulator::SetBeatFrequency(float freq) {
        beatFrequency = freq;
        ComputeTrueFrequency();
        ComputePhaseIncrement();
    }

    void Modulator::SetNoteDuration(NoteDuration duration) {
        switch(duration) {
            case BEAT:
                currentNoteDurationFactor = 1.0;
                break;
            case HALF_NOTE:
                currentNoteDurationFactor = 0.5;
                break;
            case EIGHTH:
                currentNoteDurationFactor = 2.0;
                break;
            case SIXTEENTH:
                currentNoteDurationFactor = 4.0;
                break;
            case BEAT_TRIPLET:
                currentNoteDurationFactor = 3.0;
                break;
            case HALF_TRIPLET:
                currentNoteDurationFactor = 1.5;
                break;
            case EIGHTH_TRIPLET:
                currentNoteDurationFactor = 6.0;
                break;
            case SIXTEENTH_TRIPLET:
                currentNoteDurationFactor = 12.0;
                break;
        }
        ComputeTrueFrequency();
        ComputePhaseIncrement();
    }

    void Modulator::SetAmplitude(float val) {
        this->amplitude = val;
    }

    void Modulator::SetOffset(float val) {
        this->offset = val;
    }

    void Modulator::SetWaveToSin() {
        wavetable = sintable;
    }

    void Modulator::SetWaveToTri() {
        wavetable = tritable;
    }

    void Modulator::SetWaveToSaw() {
        wavetable = sawtable;
    }

    void Modulator::HardSync() {
        this->phase_acc = 0.0;
    }
}