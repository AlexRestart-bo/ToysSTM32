# Toolchain
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size

# Microcontroller
MCU = -mcpu=cortex-m3 -mthumb

# Optimization
OPT = -Og

# Paths
ROOT = .
CORE = $(ROOT)/core

# Source files
C_SOURCES = \
$(CORE)/src/main.c \
$(CORE)/src/system_stm32f1xx.c \
$(CORE)/src/melodies.c 
#$(CORE)/src/usart.c 

# ASM sources
ASM_SOURCES = \
$(CORE)/startup/startup_stm32f103xb.s

# Include paths
C_INCLUDES = \
-I$(CORE)/inc/

# Compiler flags
CFLAGS = $(MCU) $(OPT) -std=gnu11 \
-DSTM32F103xB \
$(C_INCLUDES) \
-ffunction-sections -fdata-sections \
-Wall -fstack-usage --specs=nano.specs

# Linker flags
LDFLAGS = $(MCU) -T"$(CORE)/STM32F103C8TX_FLASH.ld" \
--specs=nosys.specs \
-Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref \
-Wl,--gc-sections

# Output files
TARGET = firmware
BUILD_DIR = build

# Default target
all: $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET).hex

# Create build directory


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
	st-flash write $< 0x8000000

.PHONY: all clean flash