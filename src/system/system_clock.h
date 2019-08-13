#ifndef CONTROLLER_SYSTEM_CLOCK_H_
#define CONTROLLER_SYSTEM_CLOCK_H_

namespace wavesynth {
    
class SystemClock {
    public:
        SystemClock() {}
        ~SystemClock() {}

        inline void Init() {count=0;}
        inline void Tick() {++count;}
        inline volatile uint32_t millis() const {return count;}
    private:
        volatile uint32_t count;
};

extern SystemClock system_clock;

}

#endif