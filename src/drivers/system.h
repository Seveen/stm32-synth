#ifndef CONTROLLER_DRIVERS_SYSTEM_H_
#define CONTROLLER_DRIVERS_SYSTEM_H_

#include <stdint.h>

namespace wavesynth {

class System {
    public:
        System() {}
        ~System() {}

        void Init();
        void StartTimers();
    
    private:
        void ClockConfig();
};

}

#endif