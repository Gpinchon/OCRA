/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:27:05
*/
#pragma once

#include <Handle.hpp>
#include <Pipeline/StageFlags.hpp>

#include <vector>

namespace OCRA {
struct AllocationCallback;
}

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::Queue::Semaphore);

namespace OCRA::Queue::Semaphore {
struct SubmitInfo {
	Handle semaphore;
	uint64_t value;
	Pipeline::StageFlags::Bits stageMask;
	uint32_t deviceIndex;
};
Handle Create(
	const Device::Handle& a_Device,
	const AllocationCallback* a_Allocator = nullptr);
void Signal(
	const Device::Handle& a_Device,
	const Handle& a_Semaphore,
	const uint64_t& a_Value);
void Signal(
	const Device::Handle& a_Device,
	const std::vector<Handle>& a_Semaphores,
	const std::vector<uint64_t>& a_Values);
bool Wait(
	const Device::Handle& a_Device,
	const std::vector<Handle>& a_Semaphores,
	const std::vector<uint64_t>& a_Values,
	const uint64_t& a_TimeoutMS);
uint64_t GetCounterValue(
	const Device::Handle& a_Device,
	const Handle& a_Semaphore);
}