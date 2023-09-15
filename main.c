#include "constants.h"
#include "misc.h"

typedef struct led_tuple {
    volatile uint32_t * base;
    uint8_t pin;
} led_tuple_t;

led_tuple_t LED_SEQUENCE[] = {
    {GPIOD_BASE, 13},
    {GPIOD_BASE, 12},
    {GPIOD_BASE, 15},
    {GPIOD_BASE, 14},
    {0, 0}
};

typedef struct vector_table {
    void *stackInit;
    void *Reset;
    void *NMI;
    void *HardFault;
    void *MemManage;
    void *BusFault;
    void *UsageFault;
    void *reserved_0[4];
    void *SVCall;
    void *DebugMonitor;
    void *reserved_1;
    void *PendSV;
    void *SysTick;
    void *extra1[6];
    void *EXTI0;
    void *extra2[256-16-7];
} vector_table_t;

void _start();
void _systick();
void _exti0();

vector_table_t vector_table __attribute__ ((section ("VECTOR_TABLE"))) ={
    .stackInit = (void *)MEM_END,
    .Reset = _start+1,
    .SysTick = _systick+1,
    .EXTI0 = _exti0+1,
};

bool phase = 0;
bool btnDown = false;
void _systick() {
    if (!phase) {
        setRegisterBits(EXTI_IMR, 0, 1, 1); //Unmask EXTI0
        if (btnDown != gpioRead(GPIOA_BASE, 0)) {
            _exti0();
            return;
        }
        if (btnDown) setRegisterBits(STK_CTRL, 0, 1, 0); //Disable counter
        else setRegisterBits(STK_LOAD, 0, 24, 2000000*0.25 - 20000);
        phase = 1;
    } else {
        gpioWrite(LED_SEQUENCE[0].base, LED_SEQUENCE[0].pin, false);
        setRegisterBits(STK_CTRL, 0, 1, 0); //Disable counter
    }
    setRegisterBits(STK_VAL, 0, 1, 1); //Reset timer to clear interrupt
}

void _exti0() {
    btnDown = gpioRead(GPIOA_BASE, 0);
    *EXTI_PR = 1 << 0; // Clear EXTI peding interrupt register
    *NVIC_ICPRx(0) = 1 << 6; // Clear NVIC peding interrupt register
    phase = 0;
    if (btnDown) {
        gpioWrite(LED_SEQUENCE[0].base, LED_SEQUENCE[0].pin, true);
    }
    setRegisterBits(STK_LOAD, 0, 24, 20000); //Debounce for 10 ms
    setRegisterBits(STK_VAL, 0, 1, 1); //Reset timer
    setRegisterBits(STK_CTRL, 0, 1, 1); //Enable counter
    setRegisterBits(EXTI_IMR, 0, 1, 0); //Mask EXTI0
}

void _start() {
    // Clock stuff
    *RCC_AHB1ENR |= 1 << 3 | 1 << 0;
    *RCC_APB1ENR |= 1 << 17;

    // Set GPIOA0 to input and pull-down
    gpioSetInput(GPIOA_BASE, 0, 0b10);
    // Expose USART2_TX and USART2_RX
    gpioSetAlternateFunction(GPIOA_BASE, 2, 7);
    gpioSetAlternateFunction(GPIOA_BASE, 3, 7);

    //Configure SysTick
    setRegisterBits(STK_CTRL, 1, 1, 1); //SysTick exception request enabled
    setRegisterBits(STK_CTRL, 2, 1, 0); //Systick source: AHB/8 (AHB = HSI (16 MHz))

    // Configure USART2 (8N1)
    setRegisterBits(USARTx_CR1(USART2_BASE), 13, 1, 1); // UE = 1
    setRegisterBits(USARTx_CR1(USART2_BASE), 12, 1, 0); // M = 0
    setRegisterBits(USARTx_CR2(USART2_BASE), 12, 2, 0); // Stop bits = 1
    setRegisterBits(USARTx_CR1(USART2_BASE), 15, 1, 0); // OVER8 = 0
    *USARTx_BRR(USART2_BASE) = 0b10001011; // Baud rate divisor for 115200 Hz bitrate, 16 MHz clock, and oversampling by 16
    setRegisterBits(USARTx_CR1(USART2_BASE), 3, 1, 1); // TE = 1

    usartWriteString(USART2_BASE, "Hello World!💖");

    // Configure interrupt on GPIOA0
    setRegisterBits(SYSCFG_EXTICR1, 0, 4, 0b0000); //Select GPIOA0 as the source input for EXTI0
    setRegisterBits(EXTI_IMR, 0, 1, 1); //Unmask EXTI0
    setRegisterBits(EXTI_RTSR, 0, 1, 1); //Set EXTI0 to fire on a rising edge
    setRegisterBits(EXTI_FTSR, 0, 1, 1); //Set EXTI0 to fire on a falling edge
    *NVIC_ISERx(0) = 1 << 6; // Enable NVIC line 6 interrupt

    for (led_tuple_t *ledPtr = LED_SEQUENCE; ledPtr->base; ledPtr += 1) {
        // Set GPIODX to output
        gpioSetOutput(ledPtr->base, ledPtr->pin);
    }

    // Spin
    while (true) {

    }
}
