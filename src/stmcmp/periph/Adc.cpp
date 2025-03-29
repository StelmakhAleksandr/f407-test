#include "Adc.h"

void setupAdc(const AdcCycles &cycles)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    GPIOA->MODER |= GPIO_MODER_MODER0; // 11
    ADC1->SQR3 = 0;
    uint32_t speed = (uint32_t)cycles;
    ADC1->SMPR2 = ADC1->SMPR2 & ~ADC_SMPR2_SMP0_Msk | (speed << ADC_SMPR2_SMP0_Pos);
    ADC1->SR &= ~ADC_SR_EOC;
    ADC1->CR1 |= ADC_CR1_EOCIE;
    ADC1->CR2 |= ADC_CR2_ADON;
    NVIC_EnableIRQ(ADC_IRQn);
}

void startAdc()
{
    ADC1->CR2 |= (ADC_CR2_SWSTART | ADC_CR2_CONT);
}

void stopAdc()
{
    ADC1->CR2 &= ~(ADC_CR2_SWSTART | ADC_CR2_CONT);
}