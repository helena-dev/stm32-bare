all: romboot

PREFIX = arm-none-eabi-

code.o: code.S constants.S
	$(PREFIX)gcc -c -mcpu=cortex-m4 $< -o $@

code.bin: code.o
	$(PREFIX)objcopy -O binary $< $@

_foony:
romboot: code.bin _foony
	openocd -f board/stm32f4discovery.cfg -c "init" -c "reset init" -c "flash write_image erase code.bin 0x08000000 bin" -c "reset run"

clean:
	rm -rf *.bin *.o
