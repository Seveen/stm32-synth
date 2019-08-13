#ifndef CONTROLLER_EVENT_QUEUE_H_
#define CONTROLLER_EVENT_QUEUE_H_

#include "utils/ring_buffer.h"

namespace wavesynth {

enum ControlType {
    CONTROL_POT = 0,
    CONTROL_ENCODER = 1,
    CONTROL_ENCODER_CLICK = 2,
    CONTROL_ENCODER_LONG_CLICK = 3,
    CONTROL_SWITCH = 4,
    CONTROL_SWITCH_HOLD = 5,
    ENVELOPE_UPDATE = 6,
    MODULATOR_UPDATE= 7,
    CONTROL_REFRESH = 0xff
};

struct Event {
    ControlType control_type;
    uint16_t control_id;
    int32_t data;
};

template<uint16_t size = 32>
class EventQueue {
    public :
        EventQueue() {}

        void Init() {
            events.Init();
        }

        void Flush() {
            events.Flush();
        }

        void AddEvent(ControlType control_type, uint16_t id, int32_t data) {
            Event e;
            e.control_type = control_type;
            e.control_id = id;
            e.data = data;
            events.Overwrite(e);
        }

        uint32_t available() {
            return events.readable();
        }

        Event PullEvent() {
            return events.ImmediateRead();
        }

    private:
        RingBuffer<Event, size> events;
};

}

#endif