#include "tempo_keeper.h"
#include "system/system_clock.h"

namespace wavesynth {
    void TempoKeeper::Init() {
        period = 0;
        lastTap = 0;
    }
    
    void TempoKeeper::Tap() {
        uint32_t now = system_clock.millis();
        period = now - lastTap;
        lastTap = now;
    }

    bool TempoKeeper::isNowHalfNote() {
        uint32_t now = system_clock.millis();
        return (lastTap + now) % (period << 1) == 0;
    }

    bool TempoKeeper::isNowBeat() {
        uint32_t now = system_clock.millis();
        return (lastTap + now) % period == 0;
    }

    bool TempoKeeper::isNowEighth() {
        uint32_t now = system_clock.millis();
        return (lastTap + now) % (period >> 1) == 0;
    }

    bool TempoKeeper::isNowSixteenth() {
        uint32_t now = system_clock.millis();
        return (lastTap + now) % (period >> 2) == 0;
    }

    bool TempoKeeper::isNowHalfNoteTriplet() {
        uint32_t now = system_clock.millis();
        return (lastTap + now) % ((period << 1)/3) == 0;
    }

    bool TempoKeeper::isNowBeatTriplet() {
        uint32_t now = system_clock.millis();
        return (lastTap + now) % (period/3) == 0;
    }

    bool TempoKeeper::isNowEighthTriplet() {
        uint32_t now = system_clock.millis();
        return (lastTap + now) % ((period >> 1)/3) == 0;
    }

    bool TempoKeeper::isNowSixteenthTriplet() {
        uint32_t now = system_clock.millis();
        return (lastTap + now) % ((period >> 2)/3) == 0;
    }

    float TempoKeeper::getTempoFrequency() {
        return 1000.0/period;
    }
} 