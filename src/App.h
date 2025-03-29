#pragma once

#include "stmcmp/stmcmp.h"

class App
{
public:
    void setup();
    void run();

private:
    void ledTask(GpioPin led, int intervalMs);
    void helloTask();

    GpioPin m_ledOrange = GpioBuilder::none();
    GpioPin m_ledGreen = GpioBuilder::none();
    GpioPin m_ledRed = GpioBuilder::none();
    GpioPin m_ledBlue = GpioBuilder::none();
};