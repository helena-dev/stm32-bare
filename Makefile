all: romboot

PREFIX = arm-none-eabi-

code.o: code.S constants.S
	$(PREFIX)gcc -c -mcpu=cortex-m4 $< -o $@

code.elf: code.lds code.o
	$(PREFIX)ld -T $^ -o $@

_foony:
romboot: code.elf _foony
	openocd -f board/stm32f4discovery.cfg -c "program code.elf preverify verify reset 0x08000000"

clean:
	rm -rf *.elf *.o
