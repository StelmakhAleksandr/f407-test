#include "Usart.h"

static USART_TypeDef *Terminal;

void setupTerminal(USART_TypeDef *usart)
{
    Terminal = usart;
}

void setupUsart(USART_TypeDef *usart, long long busSpeed, long long boud, GpioBuilder tx)
{
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
    // tx.setType(PinType::PushPull).setSpeed(PinSpeed::VeryHigh).setModer(PinModer::AF).setPull(PinPull::No).setAlternateFunction(7);
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // Включення такту GPIOC
    // Налаштування PC10 (TX) для альтернативної функції USART3
    GPIOC->MODER &= ~(GPIO_MODER_MODER10);       // Очищення бітів MODER10
    GPIOC->MODER |= (GPIO_MODER_MODER10_1);      // Альтернативна функція (10)
    GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_10);       // Push-pull (0)
    GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR10); // Дуже висока швидкість
    GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR10);       // Без підтяжки
    // Встановлення альтернативної функції AF7 (USART3) для PC10
    GPIOC->AFR[1] &= ~(0xF << (4 * 2)); // Очищення бітів для PC10 (AFR[1] відповідає PC8-PC15)
    GPIOC->AFR[1] |= (7 << (4 * 2));    // Встановлення AF7 (USART3)
    usart->BRR = busSpeed / boud;
    usart->CR1 = USART_CR1_TE | USART_CR1_UE;
    usart->CR2 = 0;
    usart->CR3 = 0;

    while (!(usart->SR & USART_SR_TC))
        ;
}

extern "C" int _write(int file, char *ptr, int len)
{
    for (int i = 0; i < len; ++i)
    {
        while ((Terminal->SR & USART_SR_TC) == 0)
        {
        }
        Terminal->DR = ptr[i];
    }
    return len;
}