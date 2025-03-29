#pragma once

#include "Pins.h"

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

void setupAdc(const AdcCycles &cycles);
void startAdc();
void stopAdc();
