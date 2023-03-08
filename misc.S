.text
.align 2
.global getRegisterBits
getRegisterBits:
    /*
    :param i32 address: Register address
    :param i5 start: start bit
    :param i5 length: number of bits to get
    :return i32: value of bits
     */

    ldr r0, [r0]
    lsr r0, r0, r1
    mov r1, #0
    mvn r1, r1
    lsl r1, r1, r2
    mvn r1, r1
    and r0, r0, r1
    bx r14

.bss
.align 4
my_variable:
.word 0
