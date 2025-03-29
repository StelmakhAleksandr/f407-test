#include "App.h"

static App *instance;
extern "C" void ADC_IRQHandler()
{
    instance->updateAdc(ADC1->DR);
    ADC1->SR &= ~ADC_SR_EOC;
}

void App::setup()
{
    ClocksBuilder().setHse(8_MHz).setSysclk(168_MHz).build();
    setupRtos();
    setupUsart(USART3, 168_MHz / 4, 115200, PC10);
    setupTerminal(USART3);

    m_ledGreen = PD12.setModer(PinModer::Output).setType(PinType::PushPull).build();
    m_ledOrange = PD13.setModer(PinModer::Output).setType(PinType::PushPull).build();
    m_ledRed = PD14.setModer(PinModer::Output).setType(PinType::PushPull).build();
    m_ledBlue = PD15.setModer(PinModer::Output).setType(PinType::PushPull).build();

    makeTask([this]
             { ledTask(m_ledGreen, 100); }, "Blink", TaskStackSize::Small, TaskPriority::Low);
    makeTask([this]
             { ledTask(m_ledOrange, 300); }, "Blink", TaskStackSize::Small, TaskPriority::Low);
    makeTask([this]
             { ledTask(m_ledRed, 500); }, "Blink", TaskStackSize::Small, TaskPriority::Low);
    makeTask([this]
             { ledTask(m_ledBlue, 1000); }, "Blink", TaskStackSize::Small, TaskPriority::Low);
    makeTask(std::bind(&App::helloTask, this), "Hello", TaskStackSize::Small, TaskPriority::Low);

    makeTask(std::bind(&App::adcTask, this), "Adc", TaskStackSize::Small, TaskPriority::High);
}

void App::run()
{
    vTaskStartScheduler();
}

void App::ledTask(GpioPin led, int intervalMs)
{
    while (true)
    {
        led.toggle();
        vTaskDelay(pdMS_TO_TICKS(intervalMs));
    }
}

void App::helloTask()
{
    while (true)
    {
        printf("Hello = %d\r\n", 100);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void App::updateAdc(uint32_t value)
{
    if (m_current >= 100000)
    {
        m_current = 0;
    }
    m_results[m_current++] = value;
}

void App::adcTask()
{
    instance = this;
    setupAdc(AdcCycles::Adc480);
    m_current = 0;

    while (true)
    {
        startAdc();
        vTaskDelay(pdMS_TO_TICKS(100));
        stopAdc();
        printf("we get %d adcd results \r\n", m_current);
        vTaskDelay(pdMS_TO_TICKS(1000));
        m_current = 0;
    }
}
