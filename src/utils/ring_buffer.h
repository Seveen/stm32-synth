#ifndef CONTROLLER_RING_BUFFER_H_
#define CONTROLLER_RING_BUFFER_H_

namespace wavesynth {

template<typename T, uint32_t size>
class RingBuffer {
    public:
        RingBuffer() {}

        inline void Init() {
            read_ptr = write_ptr = 0;
        }

        inline uint32_t capacity() const { return size; }

        inline uint32_t writable() const {
            return (read_ptr - write_ptr - 1) % size;
        }

        inline uint32_t readable() const {
            return (write_ptr - read_ptr) % size;
        }

        inline void Write(T v) {
            while (!writable());
            Overwrite(v);
        }

        inline void Overwrite(T v) {
            size_t w = write_ptr;
            buffer[w] = v;
            write_ptr = (w + 1) % size;
        }

        inline T Read() {
            while (!readable());
            return ImmediateRead();
        }

        inline T ImmediateRead() {
            size_t r = read_ptr;
            T result = buffer[r];
            read_ptr = (r + 1) % size;
            return result;
        }

        inline void Flush() {
            write_ptr = read_ptr;
        }

    private:
        T buffer[size];
        volatile uint32_t read_ptr;
        volatile uint32_t write_ptr;
};

}

#endif