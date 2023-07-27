#define __register(base, offset) ((volatile uint32_t *) (((uint32_t)(base)) + offset ))

#define GPIOA_BASE ((volatile uint32_t *)0x40020000)
#define GPIOD_BASE ((volatile uint32_t *)0x40020C00)

#define GPIOx_MODER(base) __register(base, 0x0)
#define GPIOx_PUPDR(base) __register(base, 0x0C)
#define GPIOx_IDR(base) __register(base, 0x10)
#define GPIOx_BSRR(base) __register(base, 0x18)
#define GPIOx_OTYPER(base) __register(base, 0x04)

#define RCC_BASE ((volatile uint32_t *)0x40023800)
#define RCC_AHB1ENR __register(RCC_BASE, 0x30)

#define MEM_END (0x20000000 + 0x1C000)

#define bool _Bool
#define false 0
#define true 1