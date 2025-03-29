#include "stm32f4xx_hal.h"
#include <FreeRTOS.h>
#include <task.h>
#include "../periph/Irq.h"

static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleTaskStack[configMINIMAL_STACK_SIZE * 10];
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerTaskStack[configTIMER_TASK_STACK_DEPTH * 10];
extern "C" void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = xIdleTaskStack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

extern "C" void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
    *ppxTimerTaskStackBuffer = xTimerTaskStack;
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

void setupRtos()
{
    addIrqHandler(SysTick_IRQn, []
                  {
        if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
            xPortSysTickHandler();
        } });
}