#pragma once

#include <OCRA/Core.hpp>
#include <OCRA/SwapChain/Core.hpp>

#include <vector>
#include <chrono>

namespace OCRA::SwapChain
{
/**
* Presents the current Back Buffer to the Surface
*/
void Present(
    const Queue::Handle& a_Queue,
    const SwapChainPresentInfo& a_PresentInfo);
/**
* @brief in OGL it blocks for a certain time until the semaphore & fence can be signaled,
* if it times out, a handle will be returned and the fence signaled later on.
* @return : a handle to the current Back Buffer
* In OGL, it ALWAYS returns a handle
*/
Image::Handle AcquireNextImage(
    const Device::Handle& a_Device,
    const Handle& a_SwapChain,
    const std::chrono::nanoseconds& a_Timeout,
    const Semaphore::Handle& a_Semaphore,
    const Fence::Handle& a_Fence);
}