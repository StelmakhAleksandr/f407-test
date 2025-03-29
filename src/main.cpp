#include "stm32f4xx_hal.h"
#include <string>

static volatile uint32_t adcValue = 0;
volatile int times = 0;
extern "C" void ADC_IRQHandler()
{
    adcValue = ADC1->DR;
    times++;
    ADC1->SR &= ~ADC_SR_EOC;
}

enum class AdcCycles : uint32_t
{
    Adc3 = 0b000,
    Adc15 = 0b001,
    Adc28 = 0b010,
    Adc56 = 0b011,
    Adc84 = 0b100,
    Adc112 = 0b101,
    Adc144 = 0b110,
    Adc480 = 0b111
};

// void USART3_Transmit(const std::string &message)
// {
//     for (char c : message)
//     {
//         while (!(USART3->SR & USART_SR_TXE))
//             ;           // Очікуємо, поки передавач звільниться
//         USART3->DR = c; // Передаємо символ
//     }

//     while (!(USART3->SR & USART_SR_TC))
//         ; // Чекаємо завершення передачі
// }

// int main()
// {
//     SystemClock_Config();

//     SysTick->VAL = 0;
//     SysTick->LOAD = 168000000 / 1000 - 1;
//     SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);

//     RCC->APB1ENR |= RCC_APB1ENR_USART3EN; // Включення такту USART3
//     RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;  // Включення такту GPIOC
//     // Налаштування PC10 (TX) для альтернативної функції USART3
//     GPIOC->MODER &= ~(GPIO_MODER_MODER10);       // Очищення бітів MODER10
//     GPIOC->MODER |= (GPIO_MODER_MODER10_1);      // Альтернативна функція (10)
//     GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_10);       // Push-pull (0)
//     GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR10); // Дуже висока швидкість
//     GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR10);       // Без підтяжки
//     // Встановлення альтернативної функції AF7 (USART3) для PC10
//     GPIOC->AFR[1] &= ~(0xF << (4 * 2)); // Очищення бітів для PC10 (AFR[1] відповідає PC8-PC15)
//     GPIOC->AFR[1] |= (7 << (4 * 2));    // Встановлення AF7 (USART3)

//     USART3->BRR = 168000000 / 4 / 115200; // APB1 тактується від SYSCLK/4

//     USART3->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE; // Увімкнення TX, RX і самого USART3
//     USART3->CR2 = 0;                                          // Один стоп-біт
//     USART3->CR3 = 0;                                          // Відключення додаткових опцій

//     // Очікування готовності USART3
//     while (!(USART3->SR & USART_SR_TC))
//         ;

//     RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
//     RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
//     GPIOA->MODER |= GPIO_MODER_MODER0; // 11
//     ADC1->SQR3 = 0;
//     uint32_t speed = (uint32_t)AdcCycles::Adc480;
//     ADC1->SMPR2 = ADC1->SMPR2 & ~ADC_SMPR2_SMP0_Msk | (speed << ADC_SMPR2_SMP0_Pos);
//     ADC1->SR &= ~ADC_SR_EOC;
//     ADC1->CR1 |= ADC_CR1_EOCIE;
//     ADC1->CR2 |= ADC_CR2_ADON;
//     ADC1->CR2 |= (ADC_CR2_SWSTART | ADC_CR2_CONT);
//     NVIC_EnableIRQ(ADC_IRQn);
//     delay(100);
//     auto lastTimes = times;
//     while (true)
//     {
//         auto diffTime = times - lastTimes;
//         // HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
//         std::string message = "HELLO ADC value = " + std::to_string(adcValue) + " TIMES = " + std::to_string(diffTime) + "\r\n";
//         // HAL_USART_Transmit(&usart, (uint8_t *)message.c_str(), message.size(), HAL_MAX_DELAY);
//         USART3_Transmit(message);
//         lastTimes = times;
//         delay(1000);
//     }
//     return 0;
// }

#include "App.h"

static App app;

int main()
{

    app.setup();
    app.run();
    return 0;
}