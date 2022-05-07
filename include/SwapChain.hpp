#pragma once

#include <Handle.hpp>
#include <Image/Format.hpp>
#include <Image/ColorSpace.hpp>
#include <Image/Usage.hpp>
#include <Common/Extent2D.hpp>
#include <Common/SharingMode.hpp>

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
    MaxValue
};
struct Info {
    Surface::Handle         surface{ nullptr };
    uint32_t                minImageCount{ 0 };
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
const std::vector<Image::Handle>& GetImages(const Device::Handle& a_Device, const Handle& a_SwapChain);
struct PresentInfo {
    std::vector<Queue::Semaphore::Handle>   waitSemaphores;
    std::vector<SwapChain::Handle>          swapChains;
    std::vector<uint32_t>                   imageIndices;
};
void Present(const Queue::Handle& a_Queue, const PresentInfo& a_PresentInfo);
/**
* @return : the index to the next image of the swapchain
*/
uint32_t AcquireNextImage(
    const Device::Handle& a_Device,
    const Handle& a_SwapChain,
    const std::chrono::nanoseconds& a_Timeout,
    const Queue::Semaphore::Handle& a_Semaphore,
    const Queue::Fence::Handle& a_Fence);
}