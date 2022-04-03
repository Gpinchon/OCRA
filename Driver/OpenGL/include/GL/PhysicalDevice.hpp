#pragma once

#include <PhysicalDevice.hpp>

#include <functional>

OCRA_DECLARE_HANDLE(OCRA::Instance);

namespace OCRA::PhysicalDevice
{
using Command = std::function<void()>;
Handle Create(const Instance::Handle& a_Instance);
void PushCommand(
	const Handle& a_PhysicalDevice,
	const uint32_t& a_FamilyIndex,
	const uint32_t& a_QueueIndex,
	const Command& a_Command,
	const bool a_Synchronous);
}