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
    void *extra[256-16];
} vector_table_t;

void _start();
void _systick();

vector_table_t vector_table __attribute__ ((section ("VECTOR_TABLE"))) ={
    .stackInit = (void *)MEM_END,
    .Reset = _start+1,
    .SysTick = _systick+1,
};

uint32_t phase = 1; //It do be like that sometimes
void _systick() {
    if (!phase) {
        for (uint32_t i = 0; i < 4; i++) {
            gpioWrite(LED_SEQUENCE[i].base, LED_SEQUENCE[i].pin, false);
        }
        setRegisterBits(STK_LOAD, 0, 24, 2000000);
    } else {
        gpioWrite(LED_SEQUENCE[phase-1].base, LED_SEQUENCE[phase-1].pin, true);
        setRegisterBits(STK_LOAD, 0, 24, 2000000/(phase*phase));
    }
    setRegisterBits(STK_VAL, 0, 1, 1);
    phase++;
    if (phase == 5) phase = 0;
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
    setRegisterBits(STK_LOAD, 0, 24, 1000000); //Set  Reload value
    setRegisterBits(STK_CTRL, 1, 1, 1); //SysTick exception request enabled
    setRegisterBits(STK_CTRL, 2, 1, 0); //Systick source: AHB/8 (AHB = HSI (16 MHz))
    setRegisterBits(STK_CTRL, 0, 1, 1); //Enable counter

    // Configure USART2 (8N1)
    setRegisterBits(USARTx_CR1(USART2_BASE), 13, 1, 1); // UE = 1
    setRegisterBits(USARTx_CR1(USART2_BASE), 12, 1, 0); // M = 0
    setRegisterBits(USARTx_CR2(USART2_BASE), 12, 2, 0); // Stop bits = 1
    setRegisterBits(USARTx_CR1(USART2_BASE), 15, 1, 0); // OVER8 = 0
    *USARTx_BRR(USART2_BASE) = 0b10001011; // Baud rate divisor for 115200 Hz bitrate, 16 MHz clock, and oversampling by 16
    setRegisterBits(USARTx_CR1(USART2_BASE), 3, 1, 1); // TE = 1

    usartWriteString(USART2_BASE, "Hello World!💖");

    for (led_tuple_t *ledPtr = LED_SEQUENCE; ledPtr->base; ledPtr += 1) {
        // Set GPIODX to output
        gpioSetOutput(ledPtr->base, ledPtr->pin);
    }

    led_tuple_t *ledPtr = LED_SEQUENCE;
    bool flag = false;
    while (true) {
        uint32_t btnDown = gpioRead(GPIOA_BASE, 0);
        if (btnDown) {
            if (!flag) {
                flag = true;
                gpioWrite(ledPtr->base, ledPtr->pin,true);
                ledPtr += 1;
                if (!ledPtr->base) {
                    ledPtr = LED_SEQUENCE;
                }
            }
        } else {
            if (flag) {
                for (volatile uint32_t debounce = 80000; debounce > 0; debounce--) {
                }
                flag = false;
                for (led_tuple_t *ledPtrOff = LED_SEQUENCE; ledPtrOff->base; ledPtrOff += 1) {
                    gpioWrite(ledPtrOff->base, ledPtrOff->pin, false);
                }
            }
        }
    }

    // Spin
    while (true) {

    }
}
