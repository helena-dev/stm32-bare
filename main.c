#include "constants.h"
#include "misc.h"

extern uint32_t LED_SEQUENCE;

void _start() {
    // Clock stuff
    *RCC_AHB1ENR |= 1 << 3 | 1 << 0;

    // Set GPIOA0 to input and pull-down
    gpioSetInput(GPIOA_BASE, 0, 0b10);

    for (uint32_t *ledPtr = &LED_SEQUENCE; *ledPtr; ledPtr += 2) {
        // Set GPIODX to output
        gpioSetOutput(ledPtr[0], ledPtr[1]);
    }

    uint32_t *ledPtr = &LED_SEQUENCE;
    bool flag = false;
    while (true) {
        uint32_t btnDown = getRegisterBits(GPIOx_IDR(GPIOA_BASE), 0, 1);
        if (btnDown) {
            if (!flag) {
                flag = true;
                *GPIOx_BSRR(ledPtr[0]) = 0b1 << ledPtr[1];
                ledPtr += 2;
                if (!ledPtr[0]) {
                    ledPtr = &LED_SEQUENCE;
                }
            }
        } else {
            if (flag) {
                for (volatile uint32_t debounce = 80000; debounce > 0; debounce--) {
                }
                flag = false;
                for (uint32_t *firstLedPtr = &LED_SEQUENCE + 1; *firstLedPtr; firstLedPtr += 2) {
                    *GPIOx_BSRR(GPIOD_BASE) = 0b1 << ((*firstLedPtr) + 16);
                }
            }
        }
    }

    // Spin
    while (true) {

    }
}