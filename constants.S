#define GPIOA_BASE 0x40020000
#define GPIOD_BASE 0x40020C00

#define GPIOx_MODER(base) (base + 0x0)
#define GPIOx_PUPDR(base) (base + 0x0C)
#define GPIOx_IDR(base) (base + 0x10)
#define GPIOx_BSRR(base) (base + 0x18)

#define RCC_BASE 0x40023800
#define RCC_AHB1ENR (RCC_BASE + 0x30)

#define MEM_END (0x20000000 + 0x1C000)