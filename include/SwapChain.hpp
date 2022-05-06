#pragma once

#include <Handle.hpp>
#include <Image/Format.hpp>
#include <Image/ColorSpace.hpp>
#include <Image/Usage.hpp>
#include <Common/Extent2D.hpp>
#include <Common/SharingMode.hpp>

#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::SwapChain);
OCRA_DECLARE_HANDLE(OCRA::Surface);
OCRA_DECLARE_HANDLE(OCRA::Queue);
OCRA_DECLARE_HANDLE(OCRA::Queue::Semaphore);
OCRA_DECLARE_HANDLE(OCRA::Image);

namespace OCRA::SwapChain
{
enum CompositeAlpha {
    Unknown = -1,
    Opaque,
    PreMultiplied,
    Inherit,
    MaxValue
};
enum PresentMode {
    Immediate = 0,
    Fifo = 1,
    FifoRelaxed = -1,
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
    CompositeAlpha          compositeAlpha{ CompositeAlpha::Unknown };
    PresentMode             presentMode{ PresentMode::Immediate };
    bool                    clipped{ false };
    Handle                  oldSwapchain{ nullptr };
};
Handle Create(const Device::Handle& a_Device, const Info& a_Info);
struct PresentInfo {
    std::vector<Queue::Semaphore::Handle>   waitSemaphores;
    std::vector<SwapChain::Handle>          swapChains;
    std::vector<uint32_t>                   imageIndices;
};
const std::vector<Image::Handle>& GetImages(const Device::Handle& a_Device, const Handle& a_SwapChain);
void Present(const Queue::Handle& a_Queue, const PresentInfo& a_PresentInfo);
}