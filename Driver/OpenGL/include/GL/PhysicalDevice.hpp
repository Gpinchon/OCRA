#pragma once

#include <Handle.hpp>
#include <PhysicalDevice.hpp>

#include <GL/Common/WorkerThread.hpp>
#include <GL/WeakHandle.hpp>

#include <functional>
#include <cassert>

OCRA_DECLARE_HANDLE(OCRA::Queue);

namespace OCRA::PhysicalDevice
{
using Command = std::function<void()>;
struct Queue
{
    Queue(const void* a_DeviceHandle, const void* a_ContextHandle);
    ~Queue();
    inline void PushCommand(const Command& a_Command, const bool& a_Synchronous) {
        workerThread.PushCommand(a_Command, a_Synchronous);
    }
    WorkerThread            workerThread;
    QueueFamilyProperties   properties;
};
struct Impl
{
    Impl(void* const a_DeviceHandle);
    ~Impl();
    inline void PushCommand(
        const uint32_t& a_FamilyIndex,
        const uint32_t& a_QueueIndex,
        const Command& a_Command,
        const bool a_Synchronous)
    {
        assert(a_FamilyIndex == 0);
        assert(a_QueueIndex == 0);
        queue.PushCommand(a_Command, a_Synchronous);
    }
    void SetDeviceHandle(void* const a_DeviceHandle);
    void SetSwapInterval(const int8_t& a_SwapInterval);
    int8_t              swapInterval{ 0 };
    void*               deviceHandle;
    const void*         contextHandle;
    Queue               queue;
    Properties          properties;
    Features            features;
    MemoryProperties    memoryProperties;
};
Handle Create(void* const a_DeviceHandle);
}