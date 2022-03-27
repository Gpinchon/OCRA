#pragma once

#include <Handle.hpp>
#include <Allocator.hpp>

#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Queue::Fence);
OCRA_DECLARE_HANDLE(OCRA::Device);

namespace OCRA::Queue::Fence {
enum class Status {
	Signaled, Unsignaled
};
Handle Create(
	const Device::Handle& a_Device,
	const AllocationCallback* a_Allocator = nullptr);
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