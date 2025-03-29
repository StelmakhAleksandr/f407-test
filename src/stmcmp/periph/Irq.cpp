#include "Irq.h"

std::map<IRQn_Type, std::vector<std::function<void()>>> IrqVector;

void addIrqHandler(IRQn_Type irq, std::function<void()> func)
{
    if (IrqVector.find(irq) == IrqVector.end())
    {
        IrqVector.emplace(irq, std::vector<std::function<void()>>());
    }
    IrqVector.at(irq).push_back(func);
}

void irqHandler(IRQn_Type irq)
{
    if (IrqVector.find(irq) != IrqVector.end())
    {
        for (const auto &func : IrqVector.at(irq))
        {
            func();
        }
    }
}

extern "C" void EXTI15_10_IRQHandler()
{
    irqHandler(EXTI15_10_IRQn);
}

extern "C" void SysTick_Handler()
{
    irqHandler(SysTick_IRQn);
}
