#pragma once

#include <Handle.hpp>
#include <Device.hpp>
#include <PhysicalDevice.hpp>

#include <GL/WeakHandle.hpp>
#include <GL/PhysicalDevice.hpp>

#include <functional>
#include <map>
#include <vector>

OCRA_DECLARE_WEAK_HANDLE(OCRA::PhysicalDevice);
OCRA_DECLARE_HANDLE(OCRA::PhysicalDevice);
OCRA_DECLARE_HANDLE(OCRA::Device);

namespace OCRA::Device
{
using Command = std::function<void()>;
struct Impl
{
	Impl(const PhysicalDevice::Handle& a_PhysicalDevice, const Info& a_Info);
	inline void PushCommand(
		const uint32_t& a_FamilyIndex,
		const uint32_t& a_QueueIndex,
		const Command& a_Command,
		const bool a_Synchronous)
	{
		physicalDevice.lock()->PushCommand(a_FamilyIndex, a_QueueIndex, a_Command, a_Synchronous);
	}
	const Info info;
	const PhysicalDevice::WeakHandle physicalDevice;
	std::map<uint32_t, std::vector<Queue::Handle>> queueFamilies;
};
}