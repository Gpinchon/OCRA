#pragma once

#include <Handle.hpp>
#include <Device.hpp>
#include <Memory/AllocationCallback.hpp>

#include <vector>

HANDLE(OCRA::Queue::Fence);

namespace OCRA::Queue::Fence {
enum class Status {
	Signaled, Unsignaled
};
Handle Create(
	const Device::Handle& a_Device,
	const Memory::AllocationCallback* a_Allocator = nullptr);
/**
 * @brief Used to wait for one or more fences to enter signal state
 * @arg a_WaitAll : if true, will wait for all fences to signal, else will wait for at least one fence to signal
 * @return false if timeout was reached
 */
bool WaitFor(
	const Device::Handle& a_Device,
	const std::vector<Handle>& a_Fences,
	bool a_WaitAll, uint64_t timeout);
void Reset(
	const Device::Handle& a_Device,
	const std::vector<Handle>& a_Fences);
Status GetStatus(
	const Device::Handle& a_Device,
	const Handle& a_Fence);
}