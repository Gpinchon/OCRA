#pragma once

#include <Handle.hpp>

OCRA_DECLARE_HANDLE(OCRA::Queue);
OCRA_DECLARE_HANDLE(OCRA::Device);

namespace OCRA::Queue {
Handle Create(
	const Device::Handle& a_PhysicalDevice,
	const uint32_t& a_FamilyIndex,
	const uint32_t& a_QueueIndex);
};