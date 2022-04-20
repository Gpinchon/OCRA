#pragma once

#include <Handle.hpp>
#include <PhysicalDevice.hpp>

#include <GL/Common/WorkerThread.hpp>
#include <GL/WeakHandle.hpp>

#include <functional>
#include <cassert>

OCRA_DECLARE_HANDLE(OCRA::Instance);
OCRA_DECLARE_HANDLE(OCRA::Queue);

namespace OCRA::PhysicalDevice
{
using Command = std::function<void()>;
struct QueueFamily
{
    QueueFamily(const void* a_DisplayHandle, const void* a_ContextHandle);
    ~QueueFamily();
    const void*             displayHandle;
    const void*             contextHandle;
    WorkerThread            queue;
    QueueFamilyProperties   properties;
};
struct Impl
{
    Impl(const void* a_DeviceHandle);
    ~Impl();
    inline void PushCommand(
        const uint32_t& a_FamilyIndex,
        const uint32_t& a_QueueIndex,
        const Command& a_Command,
        const bool a_Synchronous)
    {
        assert(a_FamilyIndex == 0);
        assert(a_QueueIndex == 0);
        queueFamily.queue.PushCommand(a_Command, a_Synchronous);
    }
    const void*         deviceHandle;
    const void*         displayHandle;
    const void*         contextHandle;
    const int*          attribList;
    QueueFamily         queueFamily;
    Properties          properties;
    Features            features;
    MemoryProperties    memoryProperties;
};
Handle Create(const void* a_DisplayHandle);
}