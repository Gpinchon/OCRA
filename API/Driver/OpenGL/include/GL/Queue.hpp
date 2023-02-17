#pragma once

#include <OCRA/Handle.hpp>

#include <GL/Device.hpp>

#include <functional>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Queue {
using Command = std::function<void()>;
struct Impl
{
    Impl(const Device::Handle& a_PhysicalDevice, const uint32_t& a_FamilyIndex, const uint32_t& a_QueueIndex)
        : device(a_PhysicalDevice)
        , familyIndex(a_FamilyIndex)
        , queueIndex(a_QueueIndex)
    {}
    inline void PushCommand(const Command& a_Command, const bool a_Synchronous) {
        device.lock()->PushCommand(familyIndex, queueIndex, a_Command, a_Synchronous);
    }
    inline void WaitIdle() {
        device.lock()->PushCommand(familyIndex, queueIndex, []{}, true);
    }
    const Device::WeakHandle device;
    const uint32_t  familyIndex;
    const uint32_t  queueIndex;
};
}
