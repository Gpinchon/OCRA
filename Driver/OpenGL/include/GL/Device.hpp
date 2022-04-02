#pragma once

#include <Handle.hpp>

#include <functional>

OCRA_DECLARE_HANDLE(OCRA::PhysicalDevice);
OCRA_DECLARE_HANDLE(OCRA::Device);

namespace OCRA::Device
{
using Command = std::function<void()>;
void PushCommand(
	const Handle& a_Device,
	const uint32_t& a_FamilyIndex,
	const uint32_t& a_QueueIndex,
	const Command& a_Command);
PhysicalDevice::Handle GetPhysicalDevice(const Handle& a_Device);
}