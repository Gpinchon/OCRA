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
    enum class Type {
        Win32,
        MaxValue
    };
    Impl(const Type& a_Type, const Instance::Handle& a_Instance, const void* a_ContextHandle);
    inline void PushCommand(
        const Command& a_Command,
        const bool a_Synchronous)
    {
        queue.PushCommand(a_Command, a_Synchronous);
    }
    void GetProperties();
    void ResetSurface();
    virtual void SetSurface(const Surface::Handle& a_Surface) = 0;
    virtual void SwapBuffers(const uint8_t a_SwapInterval) = 0;
    const Instance::WeakHandle instance;
    const Type          type;
    const void*         contextHandle;
    Surface::WeakHandle currentSurface;
    int8_t              swapInterval{ -2 };
    Queue               queue;
    Properties          properties;
    Features            features;
    MemoryProperties    memoryProperties;
};
}
