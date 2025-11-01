# ========================================
# MICROCONTROLLER CONFIGURATION
# ========================================
MCU = atmega328p
F_CPU = 16000000UL	# 16MHz clock source

# ========================================
# PROJECT CONFIGURATION
# ========================================
NAME = master
SLAVE_NAME = slave
SRC = $(NAME).c utils.c
SLAVE_SRC = $(SLAVE_NAME).c utils.c

# ========================================
# SERIAL COMMUNICATION
# ========================================
PORT ?= /dev/ttyUSB0	# Serial port (Linux default)
BAUD_RATE = 115200		# Baud rate for serial communication
BOARD = arduino			# Bootloader type

# ========================================
# TOOLS CONFIGURATION
# ========================================
COMPILER = avr-gcc
CONVERTER_TOOL = avr-objcopy
FLASHER_TOOL = avrdude
DUMPER_TOOL = avr-objdump

# ========================================
# COMPILER FLAGS
# ========================================
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os

# ========================================
# TARGETS
# ========================================

all: hex flash

$(SLAVE_NAME).bin: $(SLAVE_SRC)
	$(COMPILER) $(CFLAGS) $(SLAVE_SRC) -o $(SLAVE_NAME).bin

$(SLAVE_NAME).hex: $(SLAVE_NAME).bin
	$(CONVERTER_TOOL) -O ihex $(SLAVE_NAME).bin $(SLAVE_NAME).hex

# Compile C source into ELF binary
$(NAME).bin: $(SRC)
	$(COMPILER) $(CFLAGS) $(SRC) -o $(NAME).bin

# Convert ELF binary to Intel HEX format (required for microcontroller)
$(NAME).hex: $(NAME).bin
	$(CONVERTER_TOOL) -O ihex $(NAME).bin $(NAME).hex

# Generate HEX file from binary
hex: $(NAME).hex

# Flash firmware to microcontroller
flash: $(NAME).hex
	$(FLASHER_TOOL) -p $(MCU) -c $(BOARD) -b $(BAUD_RATE) -P $(PORT) -U flash:w:$(NAME).hex

# Disassemble binary to assembly
disasm: $(NAME).bin
	$(DUMPER_TOOL) -d $(NAME).bin > $(NAME).asm

# Clean generated files
clean:
	rm -f $(NAME).bin $(NAME).hex $(NAME).asm
	rm -f $(SLAVE_NAME).bin $(SLAVE_NAME).hex $(SLAVE_NAME).asm

slave: $(SLAVE_NAME).hex
	$(FLASHER_TOOL) -p $(MCU) -c $(BOARD) -b $(BAUD_RATE) -P $(PORT) -U flash:w:$(SLAVE_NAME).hex

.PHONY: all hex flash clean disasm