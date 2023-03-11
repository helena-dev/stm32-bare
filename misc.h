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
:param i5 length: number of bits to get
:param i32 value: bits to set
**/
void setRegisterBits(volatile uint32_t *address, uint8_t start, uint8_t length, uint32_t value);