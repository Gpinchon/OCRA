#pragma once

#include <OCRA/Handle.hpp>
#include <OCRA/Image/Format.hpp>
#include <OCRA/Image/ColorSpace.hpp>
#include <OCRA/Image/Usage.hpp>
#include <OCRA/Common/Extent2D.hpp>
#include <OCRA/Common/SharingMode.hpp>

#include <vector>
#include <chrono>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::SwapChain);
OCRA_DECLARE_HANDLE(OCRA::Surface);
OCRA_DECLARE_HANDLE(OCRA::Queue);
OCRA_DECLARE_HANDLE(OCRA::Queue::Semaphore);
OCRA_DECLARE_HANDLE(OCRA::Queue::Fence);
OCRA_DECLARE_HANDLE(OCRA::Image);

namespace OCRA::SwapChain
{
enum class PresentMode {
    Immediate, //No V-Sync
    Fifo,      //Yes V-Sync
    Mailbox,   //Present requests are queued
    MaxValue
};
struct Info {
    Surface::Handle         surface{ nullptr };
    uint32_t                imageCount{ 0 }; //You might get less depending on the implementation
    Image::Format           imageFormat{ Image::Format::Unknown };
    Image::ColorSpace       imageColorSpace{ Image::ColorSpace::Unknown };
    Extent2D                imageExtent{ 0, 0 };
    uint32_t                imageArrayLayers{ 0 };
    Image::UsageFlags       imageUsage{ Image::UsageFlagBits::None };
    SharingMode             imageSharingMode{ SharingMode::Exclusive };
    std::vector<uint32_t>   queueFamilyIndices;
    PresentMode             presentMode{ PresentMode::Immediate };
    bool                    clipped{ false };
    Handle                  oldSwapchain{ nullptr };
};
/**
* Creates a SwapChain using the given settings
* /!\ When using OGL on Win32, you HAVE to recreate a SwapChain on resize /!\
*/
Handle Create(const Device::Handle& a_Device, const Info& a_Info);
struct PresentInfo
{
    std::vector<Queue::Semaphore::Handle>   waitSemaphores;
    std::vector<SwapChain::Handle>          swapChains;
};
/**
* Presents the current Back Buffer to the Surface
*/
void Present(const Queue::Handle& a_Queue, const PresentInfo& a_PresentInfo);
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
    const Queue::Semaphore::Handle& a_Semaphore,
    const Queue::Fence::Handle& a_Fence);
}