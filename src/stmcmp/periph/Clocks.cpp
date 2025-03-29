#include "Clocks.h"
#include "stm32f407xx.h"

// void SystemClock_Config(void)
// {
//     RCC->APB1ENR |= RCC_APB1ENR_PWREN;
//     PWR->CR |= PWR_CR_VOS;
//     RCC->CR |= RCC_CR_HSEON;
//     while (!(RCC->CR & RCC_CR_HSERDY))
//         ;
//     RCC->CR &= ~RCC_CR_PLLON;
//     while (RCC->CR & RCC_CR_PLLRDY)
//         ;
//     RCC->PLLCFGR = (4 << RCC_PLLCFGR_PLLM_Pos) |   // PLLM = 4 (HSE / 4 = 2 MHz)
//                    (168 << RCC_PLLCFGR_PLLN_Pos) | // PLLN = 168 (2 MHz * 168 = 336 MHz VCO)
//                    (0 << RCC_PLLCFGR_PLLP_Pos) |   // PLLP = 2 (336 MHz / 2 = 168 MHz)
//                    (RCC_PLLCFGR_PLLSRC_HSE) |      // Джерело PLL = HSE
//                    (4 << RCC_PLLCFGR_PLLQ_Pos);    // PLLQ = 4 (Для USB повинно бути < 48 MHz)

//     RCC->CR |= RCC_CR_PLLON;
//     while (!(RCC->CR & RCC_CR_PLLRDY))
//         ;
//     FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_5WS;
//     RCC->CFGR &= ~RCC_CFGR_SW;
//     RCC->CFGR |= RCC_CFGR_SW_PLL;
//     while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
//         ;
//     RCC->CFGR |= RCC_CFGR_HPRE_DIV1;  // AHB = SYSCLK / 1 (168 MHz)
//     RCC->CFGR |= RCC_CFGR_PPRE1_DIV4; // APB1 = HCLK / 4 (42 MHz)
//     RCC->CFGR |= RCC_CFGR_PPRE2_DIV2; // APB2 = HCLK / 2 (84 MHz)
// }

namespace
{
    constexpr uint32_t HSI_FREQ = 16_MHz;
    constexpr uint32_t PLLM_MAX = 63;
    constexpr uint32_t PLLM_MIN = 2;
    constexpr uint32_t PLLN_MIN = 50;
    constexpr uint32_t PLLN_MAX = 432;
    constexpr uint32_t VCO_MIN = 1_MHz;
    constexpr uint32_t VCO_MAX = 2_MHz;
    constexpr uint32_t VCOCLK_MIN = 100_MHz;
    constexpr uint32_t VCOCLK_MAX = 432_MHz;
    constexpr PLLP PLLP_VALUES[] = {
        PLLP::Div2,
        PLLP::Div4,
        PLLP::Div6,
        PLLP::Div8};

    constexpr uint32_t pllToDiv(PLLP p)
    {
        switch (p)
        {
        case PLLP::Div2:
            return 2;
        case PLLP::Div4:
            return 4;
        case PLLP::Div6:
            return 6;
        case PLLP::Div8:
            return 8;
        default:
            return 2;
        }
    }
}

unsigned long long Clocks::pclk1() const { return m_pclk1; }
unsigned long long Clocks::pclk2() const { return m_pclk2; }
unsigned long long Clocks::sysclk() const { return m_sysclk; }

void Clocks::runHse()
{
    RCC->CR |= RCC_CR_HSEON;
    while ((RCC->CR & RCC_CR_HSERDY) == 0)
    {
    }
}

void Clocks::runPll()
{
    RCC->CR |= RCC_CR_PLLON;
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }
}

void Clocks::runPllclk()
{
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL)
    {
    }
}

void Clocks::setupPll(uint32_t m, uint32_t n, PLLP p)
{
    RCC->PLLCFGR = 0;
    RCC->PLLCFGR |= m_hse.has_value() ? RCC_PLLCFGR_PLLSRC_HSE : RCC_PLLCFGR_PLLSRC_HSI;
    RCC->PLLCFGR |= (m << RCC_PLLCFGR_PLLM_Pos);
    RCC->PLLCFGR |= (n << RCC_PLLCFGR_PLLN_Pos);
    RCC->PLLCFGR |= ((uint32_t)p << RCC_PLLCFGR_PLLP_Pos);
    RCC->PLLCFGR |= (4 << RCC_PLLCFGR_PLLQ_Pos);
}

void Clocks::setupBusPrescalers()
{
    RCC->CFGR &= ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2);
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2 | RCC_CFGR_HPRE_DIV1;
}

void Clocks::setupFlash()
{
    FLASH->ACR = FLASH_ACR_LATENCY_5WS | FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN;
}

void Clocks::setSysTickSpeed(unsigned long long speed)
{
    SysTick->VAL = 0;
    SysTick->LOAD = m_sysclk / speed - 1;
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
}

ClocksBuilder &ClocksBuilder::setHclk(unsigned long long value)
{
    m_clock.m_hclk = value;
    return *this;
}
ClocksBuilder &ClocksBuilder::setPclk1(unsigned long long value)
{
    m_clock.m_pclk1 = value;
    return *this;
}
ClocksBuilder &ClocksBuilder::setPclk2(unsigned long long value)
{
    m_clock.m_pclk2 = value;
    return *this;
}
ClocksBuilder &ClocksBuilder::setSysclk(unsigned long long value)
{
    m_clock.m_sysclk = value;
    return *this;
}
ClocksBuilder &ClocksBuilder::setTimclk1(unsigned long long value)
{
    m_clock.m_timclk1 = value;
    return *this;
}
ClocksBuilder &ClocksBuilder::setTimclk2(unsigned long long value)
{
    m_clock.m_timclk2 = value;
    return *this;
}
ClocksBuilder &ClocksBuilder::setHse(unsigned long long value)
{
    m_clock.m_hse = value;
    return *this;
}

ClocksBuilder &ClocksBuilder::setSysTickSpeed(unsigned long long speed)
{
    m_sysTickSpeed = speed;
    return *this;
}

Clocks ClocksBuilder::build()
{
    uint32_t m = PLLM_MIN;
    uint32_t n = PLLN_MAX;
    std::optional<PLLP> p = std::nullopt;

    calculatePllDivs(m, n, p);

    if (m_clock.m_hse.has_value())
    {
        m_clock.runHse();
    }

    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;

    m_clock.setupPll(m, n, p.value_or(PLLP::Div2));
    m_clock.runPll();
    m_clock.setupFlash();
    m_clock.runPllclk();
    m_clock.setupBusPrescalers();
    m_clock.setSysTickSpeed(m_sysTickSpeed);

    return m_clock;
}

void ClocksBuilder::calculatePllDivs(uint32_t &m_out, uint32_t &n_out, std::optional<PLLP> &p_out)
{
    auto inputClock = m_clock.m_hse.value_or(HSI_FREQ);
    auto targetSysclk = m_clock.m_sysclk;

    for (uint32_t m = PLLM_MIN; m <= PLLM_MAX; ++m)
    {
        auto vcoIn = inputClock / m;
        if (vcoIn < VCO_MIN || vcoIn > VCO_MAX)
            continue;

        for (uint32_t n = PLLN_MIN; n <= PLLN_MAX; ++n)
        {
            auto vco = (inputClock * n) / m;
            if (vco < VCOCLK_MIN || vco > VCOCLK_MAX)
                continue;

            for (auto p_enum : PLLP_VALUES)
            {
                auto p = pllToDiv(p_enum);
                auto sysclk = vco / p;
                if (sysclk != targetSysclk)
                    continue;

                for (uint32_t q = 2; q <= 15; ++q)
                {
                    auto pll48 = vco / q;
                    if (pll48 >= 47_MHz && pll48 <= 49_MHz)
                    {
                        m_out = m;
                        n_out = n;
                        p_out = p_enum;
                        return;
                    }
                }
            }
        }
    }
    m_out = 1;
    n_out = 127;
    p_out = PLLP::Div2;
}