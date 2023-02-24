#pragma once

#include <OCRA/Handle.hpp>
#include <OCRA/Common/Application.hpp>
#include <OCRA/PhysicalDevice.hpp>

#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Instance);

namespace OCRA {
struct AllocationCallback;
}

namespace OCRA::Instance
{
struct Info
{
    Application::Info applicationInfo;
};

Handle Create(
    const Info& a_Info,
    const AllocationCallback* a_Allocator = nullptr);

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