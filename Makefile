CC = arm-none-eabi-gcc
INCLUDE_PATH = -I./cmsis/CMSIS_5/CMSIS/Core/Include/ -I./cmsis/CMSIS_5/Device/ST/cmsis-device-g4/Include/ -I./h/
CFLAGS = -mcpu=cortex-m4 $(INCLUDE_PATH) -O2 #-Wall -Werror -Wextra
LINKER_FILE = ./src/linker.ld
LDFLAGS = -T $(LINKER_FILE) -nostartfiles
OUTPUT_ELF = myprog.elf
OUTPUT_BIN = myprog.bin
FLASH_BASE = 0x8000000
CLK_CALC = clk.out

all: $(OUTPUT_BIN) $(CLK_CALC)

$(OUTPUT_BIN): $(OUTPUT_ELF)
	@echo Building $(OUTPUT_BIN)
	@arm-none-eabi-objcopy -O binary $^ $@

$(OUTPUT_ELF): build/main.o build/startup.o build/nucleoG491RE.o build/flash.o build/clock.o build/lpuart.o build/print.o build/delay.o build/power.o build/systick.o build/gpio.o build/spi.o
	@echo Builing $(OUTPUT_ELF)
	@$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

flash: $(OUTPUT_BIN)
	@echo Flashing $(OUTPUT_BIN) to board
	@st-flash --reset write $^ $(FLASH_BASE)

build/main.o: src/main.c h/*.h
	@echo Builing object $@
	@$(CC) $(CFLAGS) -c $< -o $@

build/startup.o: src/startup.c h/startup.h
	@echo Builing object $@
	@$(CC) $(CFLAGS) -c $< -o $@

build/nucleoG491RE.o: src/nucleoG491RE.c h/nucleoG491RE.h
	@echo Builing object $@
	@$(CC) $(CFLAGS) -c $< -o $@

build/flash.o: src/flash.c h/flash.h
	@echo Builing object $@
	@$(CC) $(CFLAGS) -c $< -o $@

build/clock.o: src/clock.c h/clock.h
	@echo Builing object $@
	@$(CC) $(CFLAGS) -c $< -o $@

build/lpuart.o: src/lpuart.c h/lpuart.h
	@echo Builing object $@
	@$(CC) $(CFLAGS) -c $< -o $@

build/print.o: src/print.c h/print.h
	@echo Builing object $@
	@$(CC) $(CFLAGS) -c $< -o $@

build/delay.o: src/delay.c h/delay.h
	@echo Builing object $@
	@$(CC) $(CFLAGS) -c $< -o $@

build/power.o: src/power.c h/power.h
	@echo Builing object $@
	@$(CC) $(CFLAGS) -c $< -o $@

build/systick.o: src/systick.c h/systick.h
	@echo Builing object $@
	@$(CC) $(CFLAGS) -c $< -o $@

build/gpio.o: src/gpio.c h/gpio.h
	@echo Builing object $@
	@$(CC) $(CFLAGS) -c $< -o $@

build/spi.o: src/spi.c h/spi.h
	@echo Builing object $@
	@$(CC) $(CFLAGS) -c $< -o $@

$(CLK_CALC): ./sysclk.c
	@gcc $< -o $@

inspect:
	@arm-none-eabi-objdump -h $(OUTPUT_ELF)

view:
	@arm-none-eabi-objdump -S $(OUTPUT_ELF)

clean:
	@echo "Clearing build files..."
	@rm -rf *.elf *.bin ./build/*.o ./*.out

.PHONY: all clean flash view inspect
