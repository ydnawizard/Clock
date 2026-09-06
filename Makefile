# ==============================================================================
# Toolchain Configuration
# ==============================================================================
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc
LD = $(PREFIX)gcc
OBJCOPY = $(PREFIX)objcopy
SIZE = $(PREFIX)size

# Target name
TARGET = template

# ==============================================================================
# Source Files & Directories
# ==============================================================================
SRC_DIR = src
CMSIS_DIR = CMSIS
INC_DIR = include
BUILD_DIR = obj

C_SRCS = \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/gpio_driver.c \
	$(SRC_DIR)/max_driver.c \
	$(SRC_DIR)/animation.c \
	$(SRC_DIR)/fonts.c \
	$(SRC_DIR)/clock.c \
	$(SRC_DIR)/display.c\
	$(SRC_DIR)/rtc_driver.c\
	$(CMSIS_DIR)/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c

ASM_SRCS = \
	$(CMSIS_DIR)/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f401xc.s

OBJS = $(addprefix $(BUILD_DIR)/, $(notdir $(C_SRCS:.c=.o))) \
       $(addprefix $(BUILD_DIR)/, $(notdir $(ASM_SRCS:.s=.o)))

vpath %.c $(sort $(dir $(C_SRCS)))
vpath %.s $(sort $(dir $(ASM_SRCS)))

# ==============================================================================
# Compiler & Linker Flags
# ==============================================================================
# Updated for Cortex-M4 Hardware Floating Point Unit
CPU = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16

# Corrected define to STM32F401xC
CFLAGS = $(CPU) -g -O0 -Wall -Wextra \
	 -ffunction-sections -fdata-sections \
	 -I$(INC_DIR) \
	 -I CMSIS/Core/Include \
	 -I CMSIS/Device/ST/STM32F4xx/Include \
	 -I ./CMSIS/ \
	 -DSTM32F401xC

# Updated to STM32F401xC Linker Script
LDFLAGS = $(CPU) -TSTM32F401XC_FLASH.ld \
	  -Wl,--gc-sections \
	  -specs=nano.specs -specs=nosys.specs \
	  -nostartfiles

# ==============================================================================
# Build Rules
# ==============================================================================
all: $(BUILD_DIR) $(TARGET).elf $(TARGET).bin print_size

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile C source files (order-only prerequisite on BUILD_DIR)
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile Assembly startup file (order-only prerequisite on BUILD_DIR)
$(BUILD_DIR)/%.o: %.s | $(BUILD_DIR)
	$(AS) $(CFLAGS) -c $< -o $@

# Link object files into ELF executable
$(TARGET).elf: $(OBJS)
	$(LD) $(LDFLAGS) -Wl,--start-group $^ -Wl,--end-group -o $@

# Convert ELF to raw binary image
$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

# Print final memory footprint
print_size: $(TARGET).elf
	@$(SIZE) $<

# Flash target using st-flash
flash: all
	st-flash --reset write $(TARGET).bin 0x08000000

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) *.elf *.bin

.PHONY: all flash clean print_size
