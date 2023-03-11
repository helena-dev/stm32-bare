#include "constants.h"

.section VECTOR_TABLE
.align 4
vector_table:
.word MEM_END
.word _start+1
.rept 256-2
    .word 0
.endr

.text
.align 4
.global LED_SEQUENCE
LED_SEQUENCE:
// LD3 (PD13), LD4 (PD12), LD6 (PD15), LD5 (PD14)
.word GPIOD_BASE
.word 13
.word GPIOD_BASE
.word 12
.word GPIOD_BASE
.word 15
.word GPIOD_BASE
.word 14
.word 0
.word 0
