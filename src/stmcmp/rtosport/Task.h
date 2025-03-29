#pragma once

#include <FreeRTOS.h>
#include <functional>
#include <memory>
#include <string>
#include <task.h>

enum class TaskPriority
{
    Idle = 0,
    Low = 1,
    Normal = 2,
    Default = 3,
    High = 4,
    Critical = configMAX_PRIORITIES - 1
};

enum class TaskStackSize
{
    Small = 128,
    Medium = 256,
    Default = 1024,
    Large = 2048
};

class Task
{
public:
    Task(const TaskHandle_t& handle);
    const TaskHandle_t& handle() const;

private:
    TaskHandle_t m_handle;
};

std::shared_ptr<Task> makeTask(const std::function<void()>& function, const std::string& title, TaskStackSize stackSize, TaskPriority priority);
