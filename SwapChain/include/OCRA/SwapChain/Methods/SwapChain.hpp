#pragma once

#include <OCRA/Core.hpp>
#include <OCRA/SwapChain/Core.hpp>

#include <vector>
#include <chrono>

namespace OCRA::SwapChain
{
/**
* @return the actual number of images used by the SwapChain
*/
uint32_t GetImageCount(const Handle& a_SwapChain);


/**
* @brief in OGL it blocks for a certain time until the semaphore & fence can be signaled,
* if it times out, a handle will be returned and the fence signaled later on.
* @return the handle to the next image and its index in the SwapChain
*/
std::pair<Image::Handle, uint32_t> GetNextImage(
    const Handle& a_SwapChain,
    const std::chrono::nanoseconds& a_Timeout,
    const Semaphore::Handle& a_Semaphore,
    const Fence::Handle& a_Fence);
}