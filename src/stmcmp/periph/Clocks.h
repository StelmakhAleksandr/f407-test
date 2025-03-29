#pragma once

#include <cstdint>
#include <optional>

constexpr unsigned long long operator"" _hz(unsigned long long val) { return val; }
constexpr unsigned long long operator"" _kHz(unsigned long long val) { return val * 1000; }
constexpr unsigned long long operator"" _MHz(unsigned long long val) { return val * 1000000; }

enum class PLLP
{
    Div2 = 0b00,
    Div4 = 0b01,
    Div6 = 0b10,
    Div8 = 0b11,
};

class ClocksBuilder;

class Clocks
{
    friend class ClocksBuilder;

public:
    unsigned long long pclk2() const;
    unsigned long long pclk1() const;
    unsigned long long sysclk() const;

private:
    Clocks() = default;
    unsigned long long m_hclk;
    unsigned long long m_pclk1;
    unsigned long long m_pclk2;
    unsigned long long m_sysclk;
    unsigned long long m_timclk1;
    unsigned long long m_timclk2;
    std::optional<unsigned long long> m_hse = std::nullopt;

    void runHse();
    void runPll();
    void runPllclk();
    void setupPll(uint32_t m, uint32_t n, PLLP p);
    void setupBusPrescalers();
    void setupFlash();
    void setSysTickSpeed(unsigned long long speed);
};

class ClocksBuilder
{
public:
    ClocksBuilder &setHclk(unsigned long long);
    ClocksBuilder &setPclk1(unsigned long long);
    ClocksBuilder &setPclk2(unsigned long long);
    ClocksBuilder &setSysclk(unsigned long long);
    ClocksBuilder &setTimclk1(unsigned long long);
    ClocksBuilder &setTimclk2(unsigned long long);
    ClocksBuilder &setHse(unsigned long long);
    ClocksBuilder &setSysTickSpeed(unsigned long long speed);
    Clocks build();

private:
    void calculatePllDivs(uint32_t &m, uint32_t &n, std::optional<PLLP> &p);
    Clocks m_clock;
    unsigned long long m_sysTickSpeed = 1_kHz;
};