# Module
MODULE ?= m1


# Toolchain
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
DB = $(PREFIX)gdb

# Microcontroller
MCU = -mcpu=cortex-m3 -mthumb

# Optimization
OPT = -Og

# Paths
ROOT = .

ifeq($(MODULE), m1)
	CORE = $(ROOT)/step1
else ifeq($(MODULE), m2)
	CORE = $(ROOT)/step2
else
	$(error Error: unknown module - $(MODULE))
endif

DRIV = $(ROOT)/drivers

# Source files
C_SOURCES = $(wildcard $(CORE)/src/*.c)
C_SOURCES += $(wildcard $(DRIV)/src/*.c)

# ASM sources
ASM_SOURCES = \
$(DRIV)/startup/startup_stm32f103xb.s

# Include paths
INCLUDE_DIRS = $(DRIV)/inc $(CORE)/inc
C_INCLUDES = $(foreach d, $(INCLUDE_DIRS), -I$(d))

# Compiler flags
CFLAGS = $(MCU) $(OPT) -std=gnu23 \
-DSTM32F103xB \
$(C_INCLUDES) \
-ffunction-sections -fdata-sections \
-Wall -fstack-usage --specs=nano.specs

# Linker flags
LDFLAGS = $(MCU) -T"$(DRIV)/STM32F103C8TX_FLASH.ld" \
--specs=nosys.specs \
-Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref \
-Wl,--gc-sections -lm

# Output files
TARGET = firmware
BUILD_DIR = build

ifeq($(MODULE), m1)
	BUILD_DIR = build/module1
else ifeq($(MODULE), m2)
	BUILD_DIR = build/module2
else
	$(error Error: unknown module - $(MODULE))
endif

# Default target
all: $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET).hex

# Compile C files
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

# Compile ASM files
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.s | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

# Link
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

# Create HEX
$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf
	$(CP) -O ihex $< $@

# Create BIN
$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(CP) -O binary -S $< $@

# Clean
clean:
	rm -rf $(BUILD_DIR)/*

# Flash with ST-Link
flash: $(BUILD_DIR)/$(TARGET).bin
	st-flash --reset write $< 0x8000000

debug:
	$(DB) $(BUILD_DIR)/$(TARGET).elf

help:
	@echo "There are many modules. You can define it to set 'MODULE' variable:"
	@echo "  m1 or m2 "

.PHONY: all clean flash help