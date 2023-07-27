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

    // Set GPIOA0 to input and pull-down
    gpioSetInput(GPIOA_BASE, 0, 0b10);

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