all: ramboot

PREFIX = arm-none-eabi-

CFLAGS = -O2 -mcpu=cortex-m4 -std=c99 -Wall -Wextra -g

misc.o: misc.c misc.h
	$(PREFIX)gcc -c $(CFLAGS) $< -o $@

main.o: main.c misc.h constants.h
	$(PREFIX)gcc -c $(CFLAGS) $< -o $@

code.elf: code.lds misc.o main.o
	$(PREFIX)ld -T $^ -o $@

_foony:
romboot: code.elf openocd.cfg _foony
	openocd -f openocd.cfg -c "init" -c "romboot" -c "exit"
ramboot: code.elf openocd.cfg _foony
	openocd -f openocd.cfg -c "init" -c "ramboot" -c "exit"

clean:
	rm -rf *.elf *.o
