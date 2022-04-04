#pragma once

#include <Handle.hpp>
#include <PhysicalDevice.hpp>

#include <GL/Instance.hpp>
#include <GL/WeakHandle.hpp>

#include <array>
#include <functional>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Instance);
OCRA_DECLARE_HANDLE(OCRA::Instance);
OCRA_DECLARE_HANDLE(OCRA::Queue);

namespace OCRA::PhysicalDevice
{
using Command = std::function<void()>;
struct Impl
{
	Impl(const Instance::Handle& a_Instance);
	void GetProperties();
	inline void PushCommand(
		const uint32_t& a_FamilyIndex,
		const uint32_t& a_QueueIndex,
		const Command& a_Command,
		const bool a_Synchronous)
	{
		instance.lock()->PushCommand(a_Command, a_Synchronous);
	}
	Instance::WeakHandle instance;
	Properties          properties;
	Features            features;
	MemoryProperties    memoryProperties;
	std::vector<QueueFamilyProperties> queueFamilyProperties;
};
Handle Create(const Instance::Handle& a_Instance);
}