all: ramboot

PREFIX = arm-none-eabi-

code.o: code.S constants.S
	$(PREFIX)gcc -c -mcpu=cortex-m4 $< -o $@

misc.o: misc.c misc.h
	$(PREFIX)gcc -c -O2 -mcpu=cortex-m4 $< -o $@

main.o: main.c misc.h constants.S
	$(PREFIX)gcc -c -O2 -mcpu=cortex-m4 $< -o $@

code.elf: code.lds code.o misc.o main.o
	$(PREFIX)ld -T $^ -o $@

_foony:
romboot: code.elf openocd.cfg _foony
	openocd -f openocd.cfg -c "init" -c "romboot" -c "exit"
ramboot: code.elf openocd.cfg _foony
	openocd -f openocd.cfg -c "init" -c "ramboot" -c "exit"

clean:
	rm -rf *.elf *.o
