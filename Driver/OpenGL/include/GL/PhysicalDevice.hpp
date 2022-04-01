#pragma once

#include <PhysicalDevice.hpp>

#include <functional>

namespace OCRA::PhysicalDevice
{
using Command = std::function<void()>;
Handle Create();
void PushCommand(
	const Handle& a_PhysicalDevice,
	const uint32_t& a_FamilyIndex,
	const uint32_t& a_QueueIndex,
	const Command& a_Command);
}