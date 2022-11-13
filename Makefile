all: romboot

PREFIX = arm-none-eabi-

code.elf: code.S constants.S
	$(PREFIX)cc -c -mcpu=cortex-m4 code.S -o code.elf

code.bin: code.elf
	$(PREFIX)objcopy -O binary code.elf code.bin

_foony:
romboot: code.bin _foony
	openocd -f board/stm32f4discovery.cfg -c "init" -c "reset init" -c "flash write_image erase code.bin 0x08000000 bin" -c "reset run"
