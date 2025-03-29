#pragma once

#include "Pins.h"

void setupUsart(USART_TypeDef *usart, long long busSpeed, long long boud, GpioBuilder tx);
void setupTerminal(USART_TypeDef *usart);
extern "C" int _write(int file, char *ptr, int len);