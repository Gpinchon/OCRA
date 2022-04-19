#pragma once

#include <Handle.hpp>
#include <PhysicalDevice.hpp>

#include <GL/Common/WorkerThread.hpp>
#include <GL/WeakHandle.hpp>

#include <array>
#include <functional>

OCRA_DECLARE_HANDLE(OCRA::Instance);
OCRA_DECLARE_HANDLE(OCRA::Queue);

namespace OCRA::PhysicalDevice
{
using Command = std::function<void()>;
struct QueueFamily
{
    QueueFamily(const void* a_DisplayHandle, void* a_ContextHandle);
    ~QueueFamily();
    const void*                       displayHandle;
    const void*                       contextHandle;
    const std::array<WorkerThread, 1> queues;
    QueueFamilyProperties             properties;
}
struct Impl
{
    Impl(const void* a_DeviceHandle);
    inline void PushCommand(
        const uint32_t& a_FamilyIndex,
        const uint32_t& a_QueueIndex,
        const Command& a_Command,
        const bool a_Synchronous)
    {
        queueFamiles.at(a_FamilyIndex).queues.at(a_QueueIndex)->PushCommand(a_Command, a_Synchronous);
    }
    const void*                      deviceHandle;
    const void*                      displayHandle;
    const void*                      contextHandle;
    const int*                       attribList;
    const std::array<QueueFamily, 1> queueFamiles;
    Properties                       properties;
    Features                         features;
    MemoryProperties                 memoryProperties;
};
Handle Create(const void* a_DisplayHandle);
}