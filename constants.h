#define __register(base, offset) ((volatile uint32_t *) (((uint32_t)(base)) + offset ))

#define GPIOA_BASE ((volatile uint32_t *)0x40020000)
#define GPIOD_BASE ((volatile uint32_t *)0x40020C00)

#define GPIOx_MODER(base) __register(base, 0x0)
#define GPIOx_PUPDR(base) __register(base, 0x0C)
#define GPIOx_IDR(base) __register(base, 0x10)
#define GPIOx_BSRR(base) __register(base, 0x18)
#define GPIOx_OTYPER(base) __register(base, 0x04)
#define GPIOx_AFRL(base) __register(base, 0x20)
#define GPIOx_AFRH(base) __register(base, 0x24)

#define RCC_BASE ((volatile uint32_t *)0x40023800)
#define RCC_AHB1ENR __register(RCC_BASE, 0x30)
#define RCC_APB1ENR __register(RCC_BASE, 0x40)

#define USART2_BASE ((volatile uint32_t *)0x40004400)
#define USARTx_SR(base) __register(base, 0x00)
#define USARTx_DR(base) __register(base, 0x04)
#define USARTx_BRR(base) __register(base, 0x08)
#define USARTx_CR1(base) __register(base, 0x0C)
#define USARTx_CR2(base) __register(base, 0x10)
#define USARTx_CR3(base) __register(base, 0x14)
#define USARTx_GTPR(base) __register(base, 0x18)

#define STK_BASE ((volatile uint32_t *)0xE000E010)
#define STK_CTRL __register(STK_BASE, 0x00)
#define STK_LOAD __register(STK_BASE, 0x04)
#define STK_VAL __register(STK_BASE, 0x08)
#define STK_CALIB __register(STK_BASE, 0x0C)

#define MEM_END (0x20000000 + 0x1C000)

#define bool _Bool
#define false 0
#define true 1
