#include "envelope.h"
#include "utils/math.h"
#include "wavetables/waves.h"

namespace wavesynth {
    void Envelope::Init() {
        phase_acc = 0.0;
        beatFrequency = 1.0;
        currentNoteDurationFactor = 1.0;

        SetBeatFrequency(1.0);
        SetAttack(0.1);
        SetDecay(1.0);
        
        blendLevel = 1.0;
        currentLow = antiarctable;
        currentHigh = tritable;
        cycling = false;
        fullOpen = false;
        state = DECAY;
    }

    float Envelope::Next() {
        if (fullOpen) {
            return 1.0;
        }

        if (state == STOPPED) {
            return 0.0;
        }

        if (cycling) {
            if (phase_acc < 128.0) {
                phase_acc += attack_phase_inc;
            } else {
                phase_acc += decay_phase_inc;
            }
            if (phase_acc >= 256.0) {
                phase_acc -= 256.0;
            }
        } else {
            if (state == ATTACK) {
                if (phase_acc < 128.0) {
                    phase_acc += attack_phase_inc;
                }
            } else if (state == DECAY) {
                if (phase_acc < 128.0) {
                    phase_acc = 128.0;
                } else {
                    phase_acc += decay_phase_inc;
                }
                if (phase_acc >= 256.0) {
                    phase_acc -= 256.0;
                    state = STOPPED;
                }
            }
        }
        
        uint8_t int_phase = (uint8_t)phase_acc;
        uint8_t int_phase_next = int_phase + 1;
        float frac = phase_acc - (float)int_phase;

        float resultLow = Math::Lerp(currentLow[int_phase], currentLow[int_phase_next], frac);
        float resultHigh = Math::Lerp(currentHigh[int_phase], currentHigh[int_phase_next], frac);

        float result = Math::Lerp(resultLow, resultHigh, blendLevel)/4096.0;

        return result;
    }

    void Envelope::SetBeatFrequency(float freq) {
        beatFrequency = freq/2.0;
        if (beatFrequency <= 0.0) {
            beatFrequency = 0.1;
        }
        ComputeTrueFrequency();
        ComputeAttackPhaseIncrement();
        ComputeDecayPhaseIncrement();
    }

    void Envelope::SetNoteDuration(NoteDuration duration) {
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
        ComputeAttackPhaseIncrement();
        ComputeDecayPhaseIncrement();
    }

    void Envelope::SetAttack(float attack) {
        this->attack = attack + 0.01;
        ComputeAttackPhaseIncrement();
    }

    void Envelope::SetDecay(float decay) {
        this->decay = decay + 0.01;
        ComputeDecayPhaseIncrement();
    }

    void Envelope::SetBlendLevel(float blend) {
        if (blend < 0.5) {
            currentLow = antiarctable;
            currentHigh = tritable;
            blend *= 2;
        } else {
            currentLow = tritable;
            currentHigh = arctable;
            blend -= 0.5;
            blend *= 2;
        }
        this->blendLevel = blend;
    }

    void Envelope::HardSync() {
        this->phase_acc = 0.0;
    }

    void Envelope::ToggleCycling() {
        this->cycling = !this->cycling;
    }

    void Envelope::SetCycling(bool val) {
        this->cycling = val;
    }

    void Envelope::SetInAttack() {
        this->state = ATTACK;
        HardSync();
    }

    void Envelope::SetFullOpen(bool val) {
        this->fullOpen = val;
    }

    void Envelope::SetInDecay() {
        this->state = DECAY;
    }
}