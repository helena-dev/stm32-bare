#include "misc.h"
#include "constants.h"

uint32_t getRegisterBits(volatile uint32_t *address, uint8_t start, uint8_t length) {
    uint32_t mask = ~(~0 << length);
    return (*address >> start) & mask;
}

void setRegisterBits(volatile uint32_t *address, uint8_t start, uint8_t length, uint32_t value) {
    //Assert TODO
    uint32_t mask = ~(~0 << length) << start;
    *address = (*address & ~mask) | (value << start);
}

void gpioSetInput(volatile uint32_t *base, uint8_t pin, uint8_t pullConfig) {
    setRegisterBits(GPIOx_MODER(base), pin * 2, 2, 0b00);
    setRegisterBits(GPIOx_PUPDR(base), pin * 2, 2, pullConfig);
}

void gpioSetOutput(volatile uint32_t *base, uint8_t pin) {
    setRegisterBits(GPIOx_MODER(base), pin * 2, 2, 0b01);
    setRegisterBits(GPIOx_OTYPER(base), pin, 1, 0b0);
}