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
* @return the actual number of images used by the SwapChain
*/
uint32_t GetImageCount(const Handle& a_SwapChain);

/**
* @brief in OGL it blocks for a certain time until the semaphore & fence can be signaled,
* if it times out, a handle will be returned and the fence signaled later on.
*
* @return the index of the next image to be used for rendering
*/
Image::Handle GetNextImage(
    const Handle& a_SwapChain,
    const std::chrono::nanoseconds& a_Timeout,
    const Semaphore::Handle& a_Semaphore,
    const Fence::Handle& a_Fence);

/**
* @brief in OGL it blocks for a certain time until the semaphore & fence can be signaled,
* if it times out, a handle will be returned and the fence signaled later on.
* @arg out_ImageIndex : used to return the current backbuffer index
* @return the index of the next image to be used for rendering
*/
Image::Handle GetNextImage(
    const Handle& a_SwapChain,
    const std::chrono::nanoseconds& a_Timeout,
    const Semaphore::Handle& a_Semaphore,
    const Fence::Handle& a_Fence,
    uint32_t& out_ImageIndex);
}