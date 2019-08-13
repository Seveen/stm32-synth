#ifndef UTILS_MATH_H_
#define UTILS_MATH_H_

#include <stdint.h>

namespace wavesynth {
    class Math {
        public:
            static inline float Lerp(uint16_t firstValue, uint16_t secondValue, float ratio) {
                return (firstValue + ((secondValue - firstValue) * ratio));
            }
    };
}

#endif