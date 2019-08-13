#ifndef CONTROLLER_TEMPOKEEPER_H_
#define CONTROLLER_TEMPOKEEPER_H_

#include <stdint.h>

#include <stm32f3xx_conf.h>
#include "stm32f3xx.h"

namespace wavesynth {
    class TempoKeeper {
        public:
            TempoKeeper() {}
            ~TempoKeeper() {}

            void Init();
            void Tap();

            bool isNowHalfNote();
            bool isNowBeat();
            bool isNowEighth();
            bool isNowSixteenth();

            bool isNowHalfNoteTriplet();
            bool isNowBeatTriplet();
            bool isNowEighthTriplet();
            bool isNowSixteenthTriplet();

            float getTempoFrequency();

        private:
            uint32_t period;
            uint32_t lastTap;
    };
}

#endif