#pragma once

#include <OCRA/Handles.hpp>

#include <vector>
#include <chrono>

namespace OCRA::Fence {
/**
 * @brief Used to wait for one or more fences to enter signal state
 * @arg a_WaitAll : if true, will wait for all fences to signal, else will wait for at least one fence to signal
 * @return false if timeout was reached
 */
bool WaitFor(
    const std::vector<Handle>& a_Fences,
    bool a_WaitAll,
    const std::chrono::nanoseconds& a_TimeoutNS);

/**
 * @brief Used to wait one fence to enter signal state
 * @return false if timeout was reached
 */
bool WaitFor(
    const Handle& a_Fences,
    const std::chrono::nanoseconds& a_TimeoutNS);

/**
* @brief resets the fence status to Unsignaled
*/
void Reset(const Handle& a_Fence);

/**
* @brief resets the fences status to Unsignaled
*/
void Reset(const std::vector<Handle>& a_Fences);

/**
* @return the current status of the fence
*/
FenceStatus GetStatus(const Handle& a_Fence);
}