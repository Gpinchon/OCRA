#pragma once

#include <Handle.hpp>

#include <GL/WeakHandle.hpp>

OCRA_DECLARE_HANDLE(OCRA::Queue);
OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Queue {
struct Impl
{
	Impl(
		const Device::Handle& a_PhysicalDevice,
		const uint32_t& a_FamilyIndex,
		const uint32_t& a_QueueIndex)
		: device(a_PhysicalDevice)
		, familyIndex(a_FamilyIndex)
		, queueIndex(a_QueueIndex)
	{}
	const Device::WeakHandle device;
	const uint32_t familyIndex;
	const uint32_t queueIndex;
};
};