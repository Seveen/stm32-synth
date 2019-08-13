#ifndef CONTROLLER_UI_H_
#define CONTROLLER_UI_H_

#include <stdint.h>

#include <stm32f3xx_conf.h>
#include "stm32f3xx.h"

#include "system/system_clock.h"
#include "drivers/debug_led.h"
#include "drivers/sr_timer.h"
#include "drivers/matrix.h"
#include "drivers/dac.h"
#include "drivers/adc.h"
#include "drivers/audio_out.h"
#include "ui/event_queue.h"
#include "ui/buttons_define.h"
#include "tempo_keeper.h"
#include "generator.h"
#include "envelope.h"
#include "modulator.h"

namespace wavesynth {
    class Ui {
        public:
            Ui() {}
            ~Ui() {}

            void Init(TempoKeeper* tempoKeeper, Generator* generator, Envelope* envelope, Modulator* xModulator);
            void Poll();
            void DoEvents();
            void FlushEvents();
            void ProcessNextSample();
            void InitWavetable();
        private:
            void OnSwitchPressed(const Event& e);
            void OnSwitchReleased(const Event& e);

            uint16_t buffer;

            SamplerateTimer srTimer;
            Matrix matrix;
            DLed led;
            Dac dac;
            AudioOut audioOut;
            Adc adc;

            uint8_t numCol = 8;
            uint8_t numRow = 8;

            EventQueue<16> queue;

            TempoKeeper* tempoKeeper;
            Generator* generator;
            Envelope* envelope;
            Modulator* xModulator;
    };
}

#endif