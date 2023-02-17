#pragma once

#include <OCRA/Handle.hpp>

#include <vector>
#include <chrono>

OCRA_DECLARE_HANDLE(OCRA::Queue::Fence);
OCRA_DECLARE_HANDLE(OCRA::Device);

namespace OCRA {
struct AllocationCallback;
}

namespace OCRA::Queue::Fence {
//Use this to ignore timeout and wait indefinitely
static constexpr auto IgnoreTimeout = (std::chrono::nanoseconds::max)();
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
    bool a_WaitAll,
    const std::chrono::nanoseconds& a_TimeoutNS);

/**
 * @brief Used to wait one fence to enter signal state
 * @return false if timeout was reached
 */
bool WaitFor(
    const Device::Handle& a_Device,
    const Handle& a_Fences,
    const std::chrono::nanoseconds& a_TimeoutNS);

/**
* @brief resets the fences status to Unsignaled
*/
void Reset(
    const Device::Handle& a_Device,
    const std::vector<Handle>& a_Fences);

/**
* @return the current status of the fence
*/
Status GetStatus(
    const Device::Handle& a_Device,
    const Handle& a_Fence);
}