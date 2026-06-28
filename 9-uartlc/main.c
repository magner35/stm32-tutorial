#include <stdio.h>
#include "stm32f103xb.h"

static void enable_usart(void);

int main(void)
{
    enable_usart();
    for (int counter = 0;; counter++)
    {
        printf("Hello, world %d\r\n", counter);
        for (volatile int unused = 0; unused < 1000000; unused++)
        {
        }
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
