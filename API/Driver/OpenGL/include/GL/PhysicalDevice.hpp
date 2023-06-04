#pragma once

#include <OCRA/Core.hpp>

#include <GL/Common/WorkerThread.hpp>

#include <functional>

OCRA_DECLARE_HANDLE(OCRA::Instance);
OCRA_DECLARE_WEAK_HANDLE(OCRA::Instance);

namespace OCRA::PhysicalDevice {
using Command = std::function<void()>;
struct Queue {
    Queue()
    {
        properties.queueCount                  = 1;
        properties.queueFlags                  = QueueFlagBits::Graphics | QueueFlagBits::Compute | QueueFlagBits::Transfer | QueueFlagBits::SparseBinding;
        properties.minImageTransferGranularity = { 1, 1, 1 }; // Queues supporting graphics and/or compute operations must report (1,1,1)
    };
    inline void PushCommand(const Command& a_Command, const bool& a_Synchronous)
    {
        workerThread.PushCommand(a_Command, a_Synchronous);
    }
    WorkerThread workerThread;
    QueueFamilyProperties properties;
};
struct Impl {
    Impl(const Instance::Handle& a_Instance);
    inline void PushCommand(
        const Command& a_Command,
        const bool a_Synchronous)
    {
        queue.PushCommand(a_Command, a_Synchronous);
    }
    void GetProperties();
    const Instance::WeakHandle instance;
    Queue queue;
    PhysicalDeviceProperties properties;
    PhysicalDeviceFeatures features;
    MemoryProperties memoryProperties;
};
}
