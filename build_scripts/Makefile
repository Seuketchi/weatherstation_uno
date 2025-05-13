# climb one level up from build_scripts/
PROJECT_ROOT = ..

DEVICE      = atmega328p
CLOCK       = 16000000
PROGRAMMER  = -c arduino -b 115200 -P /dev/ttyACM0
FUSES       = -U hfuse:w:0xde:m -U lfuse:w:0xff:m -U efuse:w:0x05:m

# Point at the real src/ tree
SRC_DIR        = $(PROJECT_ROOT)/src
INCLUDE_DIR    = $(SRC_DIR)/includes
PERIPHERAL_DIR = $(SRC_DIR)/peripherals

# now these wildcards will actually find your .c files
SOURCES    := $(wildcard $(SRC_DIR)/*.c) \
              $(wildcard $(PERIPHERAL_DIR)/*.c)
OBJECTS    := $(SOURCES:.c=.o)

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) \
          -I../src \
          -I../src/includes \
          -I../src/peripherals

.PHONY: all flash fuse clean disasm cpp

all: main.hex

# compile rules—now respect the ../ in paths
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(COMPILE) -c $< -o $@

$(PERIPHERAL_DIR)/%.o: $(PERIPHERAL_DIR)/%.c
	$(COMPILE) -c $< -o $@

main.elf: $(OBJECTS)
	$(COMPILE) -o $@ $(OBJECTS)

main.hex: main.elf
	@rm -f $@
	avr-objcopy -j .text -j .data -O ihex $< $@
	avr-size --format=avr --mcu=$(DEVICE) $<

flash: all
	$(AVRDUDE) -U flash:w:main.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

clean:
	rm -f main.hex main.elf $(OBJECTS)

disasm: main.elf
	avr-objdump -d main.elf

cpp:
	$(COMPILE) -E $(SRC_DIR)/main.c
