#include "misc.h"
#include "constants.h"

uint32_t getRegisterBits(volatile uint32_t *address, uint8_t start, uint8_t length) {
    uint32_t mask = ~(~0U << length);
    return (*address >> start) & mask;
}

void setRegisterBits(volatile uint32_t *address, uint8_t start, uint8_t length, uint32_t value) {
    //Assert TODO
    uint32_t mask = ~(~0U << length) << start;
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

bool gpioRead(volatile uint32_t *base, uint8_t pin) {
    return getRegisterBits(GPIOx_IDR(base), pin, 1);
}

void gpioWrite(volatile uint32_t *base, uint8_t pin, bool value){
    *GPIOx_BSRR(base) = 0b1 << (value ? pin : (pin+16));
}

void gpioSetAlternateFunction(volatile uint32_t *base, uint8_t pin, uint8_t funcNumber) {
    setRegisterBits(pin < 8 ? GPIOx_AFRL(base) : GPIOx_AFRH(base), (pin % 8) * 4, 4, funcNumber);
    setRegisterBits(GPIOx_MODER(base), pin * 2, 2, 0b10);
}

void usartWriteByte(volatile uint32_t *base, uint8_t byte) {
    while (!getRegisterBits(USARTx_SR(base), 7, 1));
    *USARTx_DR(base) = byte;
}

void usartWriteString(volatile uint32_t *base, const char *string) {
    while (*string) {
        usartWriteByte(base, *string);
        string++;
    }
    while (!getRegisterBits(USARTx_SR(base), 6, 1));
}
