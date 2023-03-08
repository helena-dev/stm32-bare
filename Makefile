all: ramboot

PREFIX = arm-none-eabi-

code.o: code.S constants.S
	$(PREFIX)gcc -c -mcpu=cortex-m4 $< -o $@

misc.o: misc.S
	$(PREFIX)gcc -c -mcpu=cortex-m4 $< -o $@

code.elf: code.lds code.o misc.o
	$(PREFIX)ld -T $^ -o $@

_foony:
romboot: code.elf openocd.cfg _foony
	openocd -f openocd.cfg -c "init" -c "romboot" -c "exit"
ramboot: code.elf openocd.cfg _foony
	openocd -f openocd.cfg -c "init" -c "ramboot" -c "exit"

clean:
	rm -rf *.elf *.o
