#include "stm32f103xb.h"
#include <stddef.h>

void __libc_init_array(void)
{
}

static void enable_usart(void);

static void enable_timer(void);

int main(void)
{
    enable_usart();
    enable_timer();
    for (;;)
    {
    }
}

static void enable_usart(void)
{
    // enable port B clock
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // configure PB10 as alternate function output push-pull 2MHz
    GPIOB->CRH = (GPIOB->CRH & ~(GPIO_CRH_MODE10_0 | GPIO_CRH_CNF10_0)) | GPIO_CRH_MODE10_1 | GPIO_CRH_CNF10_1;

    // enable USART3 clock
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

    // set baud rate
    USART3->BRR = 0x0341;

    // enable USART3 and transmitter
    USART3->CR1 = USART3->CR1 | USART_CR1_UE | USART_CR1_TE;
}

static void enable_timer(void)
{
    SystemCoreClockUpdate();

    // enable systick exception, enable systick counter
    SysTick->CTRL = SysTick->CTRL | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk;

    // set reload value to 1000 ms
    SysTick->LOAD = SystemCoreClock / 8 / 1;
}

static void send_hello(void);

void SysTick_Handler(void)
{
    send_hello();
}

static void send_string(const char *string);

static void send_hello(void)
{
    static char hello[16] = "Hello world 0\r\n";
    static uint8_t counter = 0;

    counter = (counter + 1) % 10;
    hello[12] = counter + '0';
    send_string(hello);
}

static void send_string(const char *string)
{
    for (size_t index = 0; string[index] != '\0'; index++)
    {
        char ch = string[index];

        // wait until data is transferred to the shift register
        while ((USART3->SR & USART_SR_TXE) == 0)
        {
        }

        // write data to the data register
        USART3->DR = (uint32_t)ch & 0x000000ff;
    }
}
