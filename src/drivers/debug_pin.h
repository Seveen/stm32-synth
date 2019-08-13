#ifndef CONTROLLER_DEBUG_PIN_H_
#define CONTROLLER_DEBUG_PIN_H_

#include <stm32f3xx_conf.h>

namespace wavesynth {

class DebugPin {
 public:
  DebugPin() { }
  ~DebugPin() { }

  static void Init() {
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_9, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_9, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_9, LL_GPIO_PULL_NO);
    LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_9);

    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_8, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_8, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_8, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_8, LL_GPIO_PULL_NO);
    LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_8);

    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_7, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_7, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_7, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_7, LL_GPIO_PULL_NO);
    LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_7);
  }
  static inline void High9() {
    WRITE_REG(GPIOA->BSRR, LL_GPIO_PIN_9);
  }
  static inline void Low9() {
    WRITE_REG(GPIOA->BRR, LL_GPIO_PIN_9);
  }
  static inline void High8() {
    WRITE_REG(GPIOA->BSRR, LL_GPIO_PIN_8);
  }
  static inline void Low8() {
    WRITE_REG(GPIOA->BRR, LL_GPIO_PIN_8);
  }
  static inline void High7() {
    WRITE_REG(GPIOA->BSRR, LL_GPIO_PIN_7);
  }
  static inline void Low7() {
    WRITE_REG(GPIOA->BRR, LL_GPIO_PIN_7);
  }
};

#define TIC9 DebugPin::High9();
#define TOC9 DebugPin::Low9();

#define TIC8 DebugPin::High8();
#define TOC8 DebugPin::Low8();

#define TIC7 DebugPin::High7();
#define TOC7 DebugPin::Low7();

}

#endif 