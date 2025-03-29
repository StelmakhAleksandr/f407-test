#pragma once

#include "stm32f407xx.h"
#include <optional>

enum class PinModer : uint8_t
{
    Input = 0b00,
    Output = 0b01,
    AF = 0b10,
    Analog = 0b11
};
enum class PinSpeed : uint8_t
{
    Low = 0b00,
    Medium = 0b01,
    High = 0b10,
    VeryHigh = 0b11
};

enum class PinType : uint8_t
{
    PushPull = 0,
    OpenDrain = 1,
};

enum class PinPull : uint8_t
{
    No = 0b00,
    Up = 0b01,
    Down = 0b10,
};

struct Exti
{
    bool rising;
    bool faling;
};

class GpioBuilder;
class GpioPin
{
    friend class GpioBuilder;

public:
    void toggle();
    void on();
    void off();

private:
    GpioPin(GPIO_TypeDef *port, uint8_t pin);
    GPIO_TypeDef *m_port;
    uint8_t m_pin;

    void setModer(PinModer value);
    void setSpeed(PinSpeed value);
    void setType(PinType value);
    void setPull(PinPull value);
    void setAlternateFunction(uint8_t afValue);

    void setValue(__IO uint32_t *reg, uint32_t bitsPerValue, uint32_t value, uint8_t offsetPin = 0);
    void toggleValue(__IO uint32_t *reg, uint32_t bitsPerValue, uint8_t offsetPin = 0);
};

class GpioBuilder
{
public:
    GpioBuilder(uint32_t port, uint8_t pin);
    GpioBuilder &setModer(PinModer value);
    GpioBuilder &setType(PinType value);
    GpioBuilder &setSpeed(PinSpeed value);
    GpioBuilder &setPull(PinPull value);
    GpioBuilder &setAlternateFunction(uint8_t value);
    GpioBuilder &setExti(bool rising, bool faling);
    GpioPin build();
    static GpioPin none();

private:
    uint32_t m_port;
    uint8_t m_pin;
    PinModer m_moder = PinModer::Input;
    PinSpeed m_speed = PinSpeed::Low;
    PinType m_type = PinType::PushPull;
    PinPull m_pull = PinPull::No;
    std::optional<uint8_t> m_af = std::nullopt;
    std::optional<Exti> m_exti = std::nullopt;
};