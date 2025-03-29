#pragma once

#include "Pins.h"

void setupUsart(USART_TypeDef *usart, long long busSpeed, long long boud, GpioBuilder tx);
void USART3_Transmit(const std::string &message);
extern "C" int _write(int file, char *ptr, int len);