#include "constants.S"

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
.global _start
_start:
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

ldr r5, =LED_SEQUENCE
init_leds_loop:
ldr r1, [r5]
cmp r1, #0
beq init_leds_end
    // Set GPIOD15 to output
    ldr r0, [r1]
    ldr r6, [r5, #4]
    lsl r6, r6, #1
    mov r3, #0b11
    lsl r3, r3, r6
    mvn r3, r3
    and r0, r3
    mov r3, #0b01
    lsl r3, r3, r6
    orr r0, r0, r3
    str r0, [r1]
add r5, r5, #8
b init_leds_loop
init_leds_end:

mov r4, #4
mov r3, #0
loop:
//Read the button (GPIOA0) pin
ldr r0, =GPIOx_IDR(GPIOA_BASE)
mov r1, #0
mov r2, #1
push {r3}
bl getRegisterBits
pop {r3}
cmp r0, #0

ldr r1, =GPIOx_BSRR(GPIOD_BASE)
beq if0

// Set GPIODx
cmp r3, #1
beq flag_on
mov r3, #1
ldr r5, =LED_SEQUENCE
add r5, r5, r4
ldr r7, [r5]
mov r2, #0b1
lsl r2, r2, r7
str r2, [r1]
add r4, #8
cmp r4, #36 // (#of leds)*8+4
bne led_end
mov r4, #4
led_end:
flag_on:
b endif

if0:
// Reset GPIODx
cmp r3, #0
beq flag_off
ldr r3, =80000
counter:
cmp r3, #0
beq counter_ended
sub r3, #1
b counter
counter_ended:
mov r3, #0
ldr r5, =(LED_SEQUENCE + 4)
reset_led:
ldr r7, [r5]
cmp r7, #0
beq end_reset_led
    ldr r6, =16
    add r6, r7, r6
    mov r2, #0b1
    lsl r2, r2, r6
    str r2, [r1]
add r5, r5, #8
b reset_led
end_reset_led:
flag_off:
endif:

b loop

spin:
b spin

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
