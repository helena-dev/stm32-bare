#include "misc.h"

uint32_t getRegisterBits(volatile uint32_t *address, uint8_t start, uint8_t length) {
    uint32_t mask = ~(~0 << length);
    return (*address >> start) & mask;
}

void setRegisterBits(volatile uint32_t *address, uint8_t start, uint8_t length, uint32_t value) {
    //Assert TODO
    uint32_t mask = ~(~0 << length) << start;
    *address = (*address & ~mask) | (value << start);
}