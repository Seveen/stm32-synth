#ifndef CONTROLLER_DRIVER_MATRIX_H_
#define CONTROLLER_DRIVER_MATRIX_H_

#include <stdint.h>

#define MATRIX_COL 8
#define MATRIX_ROW 8

namespace wavesynth {

class Matrix {
    public:
        Matrix() {}
        ~Matrix() {}

        void Init();
        void Debounce();

        inline bool released(uint8_t col, uint8_t row) const {
            return button_array[col][row] == 0xFE;
        }

        inline bool just_pressed(uint8_t col, uint8_t row) const {
            return button_array[col][row] == 0x7F;
        }

        inline bool pressed(uint8_t col, uint8_t row) const {
            return button_array[col][row] == 0xFF;
        }

        inline bool not_pressed(uint8_t col, uint8_t row) const {
            return button_array[col][row] == 0x00;
        }

    private:
        void InitGPIO();
        void InitTIM();
        void InitDMA();
        void InitBuffers();

        const uint16_t output_array[MATRIX_COL] = {
            0x0001,
            0x0002, 
            0x0004,
            0x0100,
            0x0200,
            0x0020,
            0x0400,
            0x0080
        };
        uint16_t input_array[MATRIX_COL];

        uint8_t button_array[MATRIX_COL][MATRIX_ROW];
        const uint16_t mask_array[MATRIX_COL] = {
            0x0001,
            0x0002,
            0x0004,
            0x0008,
            0x0010,
            0x0020,
            0x0040,
            0x0080,
        };
        
};
    
}

#endif