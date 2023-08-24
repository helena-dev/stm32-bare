#include "constants.h"
#include "misc.h"

typedef struct led_tuple {
    volatile uint32_t * base;
    uint8_t pin;
} led_tuple_t;

led_tuple_t LED_SEQUENCE[] = {
    {GPIOD_BASE, 13},
    {GPIOD_BASE, 12},
    {GPIOD_BASE, 15},
    {GPIOD_BASE, 14},
    {0, 0}
};

void _start();

void *vector_table[256] __attribute__ ((section ("VECTOR_TABLE"))) ={
    (void *)MEM_END,
    _start+1,
};

void _start() {
    // Clock stuff
    *RCC_AHB1ENR |= 1 << 3 | 1 << 0;
    *RCC_APB1ENR |= 1 << 17;

    // Set GPIOA0 to input and pull-down
    gpioSetInput(GPIOA_BASE, 0, 0b10);
    // Expose USART2_TX and USART2_RX
    gpioSetAlternateFunction(GPIOA_BASE, 2, 7);
    gpioSetAlternateFunction(GPIOA_BASE, 3, 7);

    // Configure USART2 (8N1)
    setRegisterBits(USARTx_CR1(USART2_BASE), 13, 1, 1); // UE = 1
    setRegisterBits(USARTx_CR1(USART2_BASE), 12, 1, 0); // M = 0
    setRegisterBits(USARTx_CR2(USART2_BASE), 12, 2, 0); // Stop bits = 1
    setRegisterBits(USARTx_CR1(USART2_BASE), 15, 1, 0); // OVER8 = 0
    *USARTx_BRR(USART2_BASE) = 0b10001011; // Baud rate divisor for 115200 Hz bitrate, 16 MHz clock, and oversampling by 16
    setRegisterBits(USARTx_CR1(USART2_BASE), 3, 1, 1); // TE = 1

    usartWriteString(USART2_BASE, "Hello World!💖");

    for (led_tuple_t *ledPtr = LED_SEQUENCE; ledPtr->base; ledPtr += 1) {
        // Set GPIODX to output
        gpioSetOutput(ledPtr->base, ledPtr->pin);
    }

    led_tuple_t *ledPtr = LED_SEQUENCE;
    bool flag = false;
    while (true) {
        uint32_t btnDown = gpioRead(GPIOA_BASE, 0);
        if (btnDown) {
            if (!flag) {
                flag = true;
                gpioWrite(ledPtr->base, ledPtr->pin,true);
                ledPtr += 1;
                if (!ledPtr->base) {
                    ledPtr = LED_SEQUENCE;
                }
            }
        } else {
            if (flag) {
                for (volatile uint32_t debounce = 80000; debounce > 0; debounce--) {
                }
                flag = false;
                for (led_tuple_t *ledPtrOff = LED_SEQUENCE; ledPtrOff->base; ledPtrOff += 1) {
                    gpioWrite(ledPtrOff->base, ledPtrOff->pin, false);
                }
            }
        }
    }

    // Spin
    while (true) {

    }
}
