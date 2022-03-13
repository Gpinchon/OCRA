#pragma once

#include <Handle.hpp>
#include <Common/Application.hpp>
#include <Allocator.hpp>
#include <PhysicalDevice.hpp>

#include <vector>

HANDLE(OCRA::Instance);

namespace OCRA::Instance
{
struct Info
{
	Application::Info applicationInfo;
};
Handle Create(
	const Info& a_Info,
	const AllocationCallback* a_Allocator);
/**
 * @brief Returns the type of Instance :
 *  - OpenGL
 *  - DirectX
 *  - Vulkan
 */
const std::string& GetType(const Handle& a_Instance);
const std::vector<PhysicalDevice::Handle>& EnumeratePhysicalDevice(const Instance::Handle& a_Instance);
const Info& GetInfo(const Handle& a_Instance);
}