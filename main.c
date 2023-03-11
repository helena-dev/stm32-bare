#include "constants.h"
#include "misc.h"

extern uint32_t LED_SEQUENCE;

void _start() {
    // Clock stuff
    volatile uint32_t *rccPtr = (volatile uint32_t *)(RCC_AHB1ENR);
    *rccPtr |= 1 << 3 | 1 << 0;

    // Set GPIOA0 to input
    setRegisterBits((volatile uint32_t *)GPIOx_MODER(GPIOA_BASE), 0, 2, 0b00);
    // Set GPIOA0 to pull-down
    setRegisterBits((volatile uint32_t *)GPIOx_PUPDR(GPIOA_BASE), 0, 2, 0b10);

    for (uint32_t *ledPtr = &LED_SEQUENCE; *ledPtr; ledPtr += 2) {
        // Set GPIODX to output
        setRegisterBits((volatile uint32_t *)ledPtr[0], ledPtr[1] * 2, 2, 0b01);
    }

    uint32_t *ledPtr = &LED_SEQUENCE;
    uint8_t flag = 0;
    while (1) {
        uint32_t btnDown = getRegisterBits((volatile uint32_t *)GPIOx_IDR(GPIOA_BASE), 0, 1);
        if (btnDown) {
            if (!flag) {
                flag = 1;
                *(volatile uint32_t *)GPIOx_BSRR(ledPtr[0]) = 0b1 << ledPtr[1];
                ledPtr += 2;
                if (!ledPtr[0]) {
                    ledPtr = &LED_SEQUENCE;
                }
            }
        } else {
            if (flag) {
                for (volatile uint32_t debounce = 80000; debounce > 0; debounce--) {
                }
                flag = 0;
                for (uint32_t *firstLedPtr = &LED_SEQUENCE + 1; *firstLedPtr; firstLedPtr += 2) {
                    *(volatile uint32_t *)GPIOx_BSRR(GPIOD_BASE) = 0b1 << ((*firstLedPtr) + 16);
                }
            }
        }
    }

    // Spin
    while (1) {

    }
}