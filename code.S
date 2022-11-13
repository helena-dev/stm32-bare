#include "constants.S"
.text

.align 4
vector_table:
.word 0
.word start+1
.rept 256-2
    .word 0
.endr

.align 4
start:
// Clock stuff
ldr r1, =RCC_AHB1ENR
ldr r0, [r1]
mov r3, #(1<<3 | 1<<0)
orr r0, r0, r3
str r0, [r1]

// Set GPIOA0 to input
ldr r1, =GPIOx_MODER(GPIOA_BASE)
ldr r0, [r1]
ldr r3, =~(0b11<<0)
and r0, r3
ldr r3, =(0b00<<0)
orr r0, r0, r3
str r0, [r1]

// Set GPIOA0 to pull-down
ldr r1, =GPIOx_PUPDR(GPIOA_BASE)
ldr r0, [r1]
ldr r3, =~(0b11<<0)
and r0, r3
ldr r3, =(0b10<<0)
orr r0, r0, r3
str r0, [r1]

// Set GPIOD15 to output
ldr r1, =GPIOx_MODER(GPIOD_BASE)
ldr r0, [r1]
ldr r3, =~(0b11<<30)
and r0, r3
ldr r3, =(0b01<<30)
orr r0, r0, r3
str r0, [r1]

loop:
//Read the button (GPIOA0) pin
ldr r1, =GPIOx_IDR(GPIOA_BASE)
ldr r0, [r1]
mov r3, #1
and r0, r3
cmp r0, #0

ldr r1, =GPIOx_BSRR(GPIOD_BASE)
beq if0

// Set GPIOD15
ldr r2, =(0b1<<15)
b endif

if0:
// Reset GPIOD15
ldr r2, =(0b1<<31)
endif:
str r2, [r1]

b loop

spin:
b spin
