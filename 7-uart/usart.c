#include "stm32f103xb.h"
#include <stddef.h>
#include <stdint.h>

static void enable_usart(void);
static void send_hello(void);

void start(void)
{
    enable_usart();
    for (;;)
    {
        send_hello();
        for (volatile uint32_t counter = 0; counter < 1000000; counter++)
        {
        }
    }
}

static void enable_usart(void)
{
    // enable port B clock (GPIOB)
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // configure PB10 as alternate function output push-pull 2MHz
    GPIOB->CRH = (GPIOB->CRH & ~(GPIO_CRH_MODE10 | GPIO_CRH_CNF10)) | GPIO_CRH_MODE10_1 | GPIO_CRH_CNF10_1;

    // enable USART3 clock
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

    // set baud rate (115200 for 72 MHz APB1)
    USART3->BRR = 0x0341;

    // enable USART3 and transmitter
    USART3->CR1 |= USART_CR1_UE | USART_CR1_TE;
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
    for (size_t i = 0; string[i] != '\0'; ++i)
    {
        // wait until transmit data register empty
        while ((USART3->SR & USART_SR_TXE) == 0)
            ;
        USART3->DR = (uint32_t)string[i] & 0xFFU;
    }
}
