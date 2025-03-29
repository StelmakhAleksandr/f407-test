#pragma once

#include "../periph/Pins.h"
#include <functional>
#include <map>

void addIrqHandler(IRQn_Type irq, std::function<void()> func);

extern "C" void EXTI15_10_IRQHandler();
extern "C" void SysTick_Handler();
