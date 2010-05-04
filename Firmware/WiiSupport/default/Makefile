###############################################################################
# Makefile for the project Dual Strike Firmware
###############################################################################

## General Flags
PROJECT = Dual Strike Firmware
MCU = atmega8
TARGET = DualStrike.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -std=c99                                                                              -DF_CPU=12000000UL -Os -fsigned-char
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS += 


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = DualStrike.o wiimote_extension_controller.o wiimote.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) DualStrike.hex DualStrike.eep size

## Compile
DualStrike.o: ../DualStrike.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

wiimote_extension_controller.o: ../wii/wiimote_extension_controller.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

wiimote.o: ../wii/wiimote.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) DualStrike.elf dep/* DualStrike.hex DualStrike.eep

## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

