#pragma once

#include <OCRA/Handles.hpp>

#include <GL/Common/Assert.hpp>
#include <GL/PhysicalDevice.hpp>
#include <GL/TextureBlitter.hpp>

#include <functional>
#include <map>
#include <vector>

OCRA_DECLARE_WEAK_HANDLE(OCRA::PhysicalDevice);

namespace OCRA::Device {
using Command = std::function<void()>;
struct Impl {
    Impl(const PhysicalDevice::Handle& a_PhysicalDevice);
    // Push a command to a specific queue
    inline void PushCommand(
        const uint32_t& a_FamilyIndex,
        const uint32_t& a_QueueIndex,
        const Command& a_Command,
        const bool a_Synchronous)
    {
        OCRA_ASSERT(a_FamilyIndex == 0);
        OCRA_ASSERT(a_QueueIndex == 0);
        physicalDevice.lock()->PushCommand(a_Command, a_Synchronous);
    }
    // Push a command directly to PhysicalDevice
    inline void PushCommand(
        const Command& a_Command,
        const bool a_Synchronous)
    {
        physicalDevice.lock()->PushCommand(a_Command, a_Synchronous);
    }
    const PhysicalDevice::WeakHandle physicalDevice;
    const TextureBlitter textureBlitter { *this };
    std::map<uint32_t, std::vector<Queue::Handle>> queueFamilies;
};
}
