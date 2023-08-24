#include "constants.h"

typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

/**
:param i32 address: Register address
:param i5 start: start bit
:param i5 length: number of bits to get
:return i32: value of bits
**/
uint32_t getRegisterBits(volatile uint32_t *address, uint8_t start, uint8_t length);

/**
:param i32 address: Register address
:param i5 start: start bit
:param i5 length: number of bits to set
:param i32 value: bits to set
**/
void setRegisterBits(volatile uint32_t *address, uint8_t start, uint8_t length, uint32_t value);

/**
:param i32 base: GPIO port base
:param i4 pin: pin number
:param i2 pullConfig: PUPDR config
**/
void gpioSetInput(volatile uint32_t *base, uint8_t pin, uint8_t pullConfig);

/**
:param i32 base: GPIO port base
:param i4 pin: pin number
**/
void gpioSetOutput(volatile uint32_t *base, uint8_t pin);

/**
:param i32 base: GPIO port base
:param i4 pin: pin number
:return bool: pin state
**/
bool gpioRead(volatile uint32_t *base, uint8_t pin);

/**
:param i32 base: GPIO port base
:param i4 pin: pin number
:param bool value: new pin state
**/
void gpioWrite(volatile uint32_t *base, uint8_t pin, bool value);