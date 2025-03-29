#pragma once

#include "stmcmp/stmcmp.h"

extern "C" void ADC_IRQHandler();

static uint32_t m_results[100000];

class App
{
public:
    friend void ADC_IRQHandler();
    void setup();
    void run();

private:
    void ledTask(GpioPin led, int intervalMs);
    void helloTask();
    void adcTask();
    void updateAdc(uint32_t value);

    GpioPin m_ledOrange = GpioBuilder::none();
    GpioPin m_ledGreen = GpioBuilder::none();
    GpioPin m_ledRed = GpioBuilder::none();
    GpioPin m_ledBlue = GpioBuilder::none();
    int m_current = 0;
};