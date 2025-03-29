#include "App.h"

void App::setup()
{
    ClocksBuilder().setHse(8_MHz).setSysclk(168_MHz).build();
    setupRtos();
    setupUsart(USART3, 168_MHz / 4, 115200, PC10);

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
        USART3_Transmit("Hello\r\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
