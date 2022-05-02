#pragma once

#include <Handle.hpp>
#include <PhysicalDevice.hpp>

#include <GL/Common/WorkerThread.hpp>
#include <GL/WeakHandle.hpp>

#include <functional>
#include <cassert>

OCRA_DECLARE_HANDLE(OCRA::Surface);
OCRA_DECLARE_HANDLE(OCRA::Instance);
OCRA_DECLARE_WEAK_HANDLE(OCRA::Instance);
OCRA_DECLARE_WEAK_HANDLE(OCRA::Surface);

namespace OCRA::PhysicalDevice
{
using Command = std::function<void()>;
struct Queue
{
    Queue() {
        properties.queueCount = 1;
        properties.queueFlags = QueueFlagsBits::Graphics | QueueFlagsBits::Compute | QueueFlagsBits::Transfer | QueueFlagsBits::SparseBinding;
        properties.minImageTransferGranularity = { 1, 1, 1 }; //Queues supporting graphics and/or compute operations must report (1,1,1)
    };
    inline void PushCommand(const Command& a_Command, const bool& a_Synchronous) {
        workerThread.PushCommand(a_Command, a_Synchronous);
    }
    WorkerThread            workerThread;
    QueueFamilyProperties   properties;
};
struct Impl
{
    Impl(const Instance::Handle& a_Instance);
    inline void PushCommand(
        const Command& a_Command,
        const bool a_Synchronous)
    {
        queue.PushCommand(a_Command, a_Synchronous);
    }
    void GetProperties();
    const Instance::WeakHandle instance;
    Queue                 queue;
    Properties            properties;
    Features              features;
    MemoryProperties      memoryProperties;
};
}
