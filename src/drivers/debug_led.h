#ifndef CONTROLLER_LED1_H_
#define CONTROLLER_LED1_H_

namespace wavesynth {
    class DLed {
        public:
            DLed() {}
            ~DLed() {}

            void On();
            void Off();
            void Toggle();

            void Init();

        private:
            bool state;
    };
}

#endif