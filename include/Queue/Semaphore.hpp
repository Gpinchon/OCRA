/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:27:05
*/
#pragma once

#include <Handle.hpp>
#include <Device.hpp>
#include <Pipeline/StageFlags.hpp>

#include <vector>

namespace OCRA::Memory {
struct AllocationCallback;
}

HANDLE(OCRA::Queue::Semaphore);

namespace OCRA::Queue::Semaphore {
struct SubmitInfo {
	Handle semaphore;
	uint64_t value;
	Pipeline::StageFlags::Bits stageMask;
	uint32_t deviceIndex;
};
Handle Create(
	const Device::Handle& a_Device,
	const Memory::AllocationCallback* a_Allocator = nullptr);
void Signal(
	const Device::Handle& a_Device,
	const Handle& a_Semaphore,
	const uint64_t& a_Value);
bool Wait(
	const Device::Handle& a_Device,
	std::vector<Handle> a_Semaphores,
	std::vector<uint64_t> a_Values,
	uint64_t a_TimeoutMS);
uint64_t GetCounterValue(
	const Device::Handle& a_Device,
	const Handle& a_Semaphore);
}