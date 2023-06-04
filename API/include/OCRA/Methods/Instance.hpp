#pragma once

#include <OCRA/Handles.hpp>
#include <OCRA/Structs.hpp>

#include <vector>

namespace OCRA {
Instance::Handle CreateInstance(
    const CreateInstanceInfo& a_Info,
    const AllocationCallback* a_Allocator = nullptr);
}

namespace OCRA::Instance {
/**
 * @brief Returns the type of Instance ("OpenGL", "DirectX" or "Vulkan")
 */
const std::string GetType(const Handle& a_Instance);

/**
 * @brief enumerates the available physical devices
 * With OpenGL it will return the default Graphics adapter
 */
const std::vector<PhysicalDevice::Handle> EnumeratePhysicalDevices(const Instance::Handle& a_Instance);
}
