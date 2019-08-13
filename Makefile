PACKAGES = src src/drivers src/utils src/ui src/system src/wavetables
CSRCS = $(notdir $(wildcard $(patsubst %,%/*.c,$(PACKAGES)))) 
CPPSRCS = $(notdir $(wildcard $(patsubst %,%/*.cc,$(PACKAGES))))

VPATH = $(PACKAGES)

# all the files will be generated with this name (main.elf, main.bin, main.hex, etc)
PROJ_NAME=project

# Location of the Libraries folder from the stm32f3xx Standard Peripheral Library
LL_LIB=Drivers

# Location of the linker scripts
LDSCRIPT_INC=Device/ldscripts

TOOLCHAIN_PATH ?= /
TOOLCHAIN_BIN  = $(TOOLCHAIN_PATH)bin/

# that's it, no need to change anything below this line!

###################################################

CC=$(TOOLCHAIN_BIN)arm-none-eabi-gcc
CXX=$(TOOLCHAIN_BIN)arm-none-eabi-g++
OBJCOPY=$(TOOLCHAIN_BIN)arm-none-eabi-objcopy
OBJDUMP=$(TOOLCHAIN_BIN)arm-none-eabi-objdump
SIZE=$(TOOLCHAIN_BIN)arm-none-eabi-size

CFLAGS  = -Wall -g -Os
CFLAGS += -DSTM32F303xE -DUSE_FULL_LL_DRIVER 
CFLAGS += -mlittle-endian -mcpu=cortex-m4  -mthumb
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Wl,--gc-sections -Wl,-Map=$(PROJ_NAME).map
CFLAGS += -Werror -Warray-bounds -fno-strict-aliasing
CFLAGS += -mfloat-abi=hard -specs=nano.specs -specs=nosys.specs
#-Wextra
###################################################

vpath %.c src
vpath %.a .

ROOT=$(shell pwd)

CFLAGS += -I $(LL_LIB) -I $(LL_LIB)/CMSIS/Device/ST/STM32F3xx/Include
CFLAGS += -I $(LL_LIB)/CMSIS/Include -I $(LL_LIB)/STM32F3xx_HAL_Driver/Inc -I src
CFLAGS += -I./src -I./src/drivers

CPPFLAGS = -fno-exceptions -fno-rtti -std=c++11

CSRCS += ./startup_stm32f303xe.s # add startup file to build

OBJS = $(CPPSRCS:.cc=.o) $(CSRCS:.c=.o) 

###################################################

.PHONY: lib proj

all: proj

proj: 	$(PROJ_NAME).elf

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.cc
	$(CXX) -c $(CFLAGS) $(CPPFLAGS) -o $@  $<

$(PROJ_NAME).elf: $(OBJS)
	$(CXX) $(CFLAGS) $(CPPFLAGS) $^ -o $@ -L$(LL_LIB) -lll -L$(LDSCRIPT_INC) -lm -Tstm32f3xx.ld
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin
	$(OBJDUMP) -St $(PROJ_NAME).elf >$(PROJ_NAME).lst
	$(SIZE) -A $(PROJ_NAME).elf
		
clean:
	find ./ -name '*~' | xargs rm -f	
	rm -f *.o
	rm -f src/*.o 
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin
	rm -f $(PROJ_NAME).map
	rm -f $(PROJ_NAME).lst

upload: $(PROJ_NAME).elf
	st-flash --reset  write project.bin 0x08000000
