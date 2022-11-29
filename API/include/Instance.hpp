#pragma once

#include <Handle.hpp>
#include <Common/Application.hpp>
#include <PhysicalDevice.hpp>

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
 * @brief Returns the type of Instance :
 *  - OpenGL
 *  - DirectX
 *  - Vulkan
 */
const std::string GetType(const Handle& a_Instance);
const std::vector<PhysicalDevice::Handle>& EnumeratePhysicalDevices(const Instance::Handle& a_Instance);
const Info& GetInfo(const Handle& a_Instance);
}