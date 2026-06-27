// target bluepillplus STM32F103CBT6 weactstudio

#include <stdint.h>

static void enable_port_clock(void);
static void configure_pin(void);
static void configure_sys_tick_timer(void);

#define RCC_BASE_ADDRESS 0x40021000
#define RCC_APB2ENR (*(volatile uint32_t *)(RCC_BASE_ADDRESS + 0x18))

#define RCC_APB2ENR_IOPBEN 3

#define GPIOB_BASE_ADDRESS 0x40010C00
#define GPIOB_CRL (*(volatile uint32_t *)(GPIOB_BASE_ADDRESS + 0x00))
#define GPIOB_ODR (*(volatile uint32_t *)(GPIOB_BASE_ADDRESS + 0x0C))

#define GPIOX_ODR_ODR2 2

#define GPIOX_CRL_MODE2_0 8
#define GPIOX_CRL_MODE2_1 9
#define GPIOX_CRL_CNF2_0 10
#define GPIOX_CRL_CNF2_1 11

#define STK_BASE_ADDRESS 0xE000E010
#define STK_CTRL (*(volatile uint32_t *)(STK_BASE_ADDRESS + 0x00))
#define STK_LOAD (*(volatile uint32_t *)(STK_BASE_ADDRESS + 0x04))

#define STK_CTRL_ENABLE 0
#define STK_CTRL_TICKINT 1
#define STK_CTRL_CLKSOURCE 2
#define STK_CTRL_COUNTFLAG 16

void start(void)
{

    enable_port_clock();
    configure_pin();
    configure_sys_tick_timer();

    for (;;)
    {
    }
}

static void enable_port_clock(void)
{
    // enable I/O port B clock
    RCC_APB2ENR = (1 << RCC_APB2ENR_IOPBEN);
}

static void configure_pin(void)
{
    // configure PB2 as push-pull output with 2 MHz speed
    GPIOB_CRL = (1 << GPIOX_CRL_MODE2_1);
}

static void configure_sys_tick_timer(void)
{
    // enable systick exception, enable systick counter
    STK_CTRL = (1 << STK_CTRL_ENABLE) | (1 << STK_CTRL_TICKINT) | (1 << STK_CTRL_CLKSOURCE);
    // set reload value to 500 ms
    STK_LOAD = 8000000 / 2;
}

static void toggle_pin(void);

void sys_tick_exception_handler(void)
{
    toggle_pin();
}

static void toggle_pin(void)
{
    // toggle PB2
    GPIOB_ODR ^= (1 << GPIOX_ODR_ODR2);
}
