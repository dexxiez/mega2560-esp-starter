# ATmega2560 (Keyestudio Mega + WiFi) — bare avr-gcc build, no Arduino core.

MCU      := atmega2560
F_CPU    := 16000000UL
PORT     ?= /dev/ttyUSB0
BAUD     ?= 115200
TARGET   := firmware

SRC_DIR   := src
BUILD_DIR := build

CC      := avr-gcc
OBJCOPY := avr-objcopy
SIZE    := avr-size
AVRDUDE := avrdude

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

CFLAGS := -mmcu=$(MCU) -DF_CPU=$(F_CPU) -std=gnu11 -Os \
          -Wall -Wextra -Werror=implicit-function-declaration \
          -ffunction-sections -fdata-sections -I$(SRC_DIR) -MMD -MP
LDFLAGS := -mmcu=$(MCU) -Wl,--gc-sections

ELF := $(BUILD_DIR)/$(TARGET).elf
HEX := $(BUILD_DIR)/$(TARGET).hex

.PHONY: all flash monitor size clean

all: $(HEX) size

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(ELF): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex -R .eeprom $< $@

size: $(ELF)
	$(SIZE) --format=avr --mcu=$(MCU) $<

# Mega bootloader is stk500v2 ("wiring"). DIP switches 3+4 ON for USB <-> 2560.
flash: $(HEX)
	$(AVRDUDE) -p $(MCU) -c wiring -P $(PORT) -b 115200 -D -U flash:w:$<:i

monitor:
	screen $(PORT) $(BAUD)

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)
