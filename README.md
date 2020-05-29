# Synth

This is a WIP wavetable synthesizer on a STM32F3 microcontroller.

## Installation
### Clone this repo
    git clone https://github.com/Seveen/stm32-synth.git
    cd stm32-synth

You need the STM32CubeF3 soft package from ST https://my.st.com/content/my_st_com/en/products/embedded-software/mcu-mpu-embedded-software/stm32-embedded-software/stm32cube-mcu-mpu-packages/stm32cubef3.html
    
    copy Drivers directory from ST soft package to ./Drivers
    from ./Drivers : make all

## Usage
### Compilation
    make

### Flash
    make upload
