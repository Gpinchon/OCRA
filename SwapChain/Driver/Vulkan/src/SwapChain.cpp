#include <SwapChain.hpp>
#include <Surface.hpp>
#include <Enums.hpp>

#include <VK/Device.hpp>
#include <VK/Enums.hpp>
#include <VK/Fence.hpp>
#include <VK/Flags.hpp>
#include <VK/Image.hpp>
#include <VK/Semaphore.hpp>
#include <VK/Queue.hpp>

#include <OCRA/Core.hpp>
#include <OCRA/SwapChain.hpp>

#include <cassert>

namespace OCRA::Device
{
SwapChain::Handle CreateSwapChain(
    const Device::Handle& a_Device,
    const CreateSwapChainInfo& a_Info)
{
    auto& device = *a_Device;
    VkSwapchainKHR vkSwapChain = nullptr;
    VkSwapchainCreateInfoKHR info{ VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
    info.clipped = a_Info.clipped;
    info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    info.imageArrayLayers   = a_Info.imageArrayLayers;
    info.imageColorSpace    = GetVkColorSpace(a_Info.imageColorSpace);
    info.imageExtent.height = a_Info.imageExtent.height;
    info.imageExtent.width  = a_Info.imageExtent.width;
    info.imageFormat        = GetVkFormat(a_Info.imageFormat);
    info.imageSharingMode   = GetVkSharingMode(a_Info.imageSharingMode);
    info.imageUsage         = GetVkImageUsage(a_Info.imageUsage);
    info.minImageCount = a_Info.imageCount;
    info.oldSwapchain = a_Info.oldSwapchain ? (VkSwapchainKHR)*a_Info.oldSwapchain : nullptr;
    info.queueFamilyIndexCount = a_Info.queueFamilyIndices.size();
    info.pQueueFamilyIndices   = a_Info.queueFamilyIndices.data();
    info.surface = *a_Info.surface;
    info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    info.presentMode = GetVkPresentMode(a_Info.presentMode);
    VkImageFormatProperties properties{};
    auto res = vkGetPhysicalDeviceImageFormatProperties(a_Device->physicalDevice,
        info.imageFormat, VK_IMAGE_TYPE_2D, VK_IMAGE_TILING_OPTIMAL, info.imageUsage, 0, &properties);
    assert(res == VK_SUCCESS);
    vkCreateSwapchainKHR(device, &info, nullptr, &vkSwapChain);
    auto swapChain = std::make_shared<SwapChain::Impl>(device, vkSwapChain);
    uint32_t imageCount = 0;
    vkGetSwapchainImagesKHR(device, vkSwapChain, &imageCount, nullptr);
    std::vector<VkImage> images(imageCount);
    vkGetSwapchainImagesKHR(device, vkSwapChain, &imageCount, images.data());
    swapChain->imagesMemory.resize(imageCount);
    swapChain->images.resize(imageCount);
    for (auto i = 0u; i < imageCount; ++i) {
        auto& vkImage     = images.at(i);
        auto& image       = swapChain->images.at(i);
        auto& imageMemory = swapChain->imagesMemory.at(i);
        image = new(&imageMemory) Image::Impl(device, vkImage);
    }
    return swapChain;
}
}

namespace OCRA::SwapChain
{
void Present(
    const Queue::Handle& a_Queue,
    const SwapChainPresentInfo& a_PresentInfo)
{
    const auto swapChainCount = a_PresentInfo.swapChains.size();
    std::vector<VkSwapchainKHR> swapChains(swapChainCount);
    std::vector<uint32_t> imageIndice(swapChainCount);
    for (auto i = 0u; i < swapChainCount; ++i) {
        const auto& swapChain = *a_PresentInfo.swapChains.at(i);
        swapChains.at(i) = swapChain;
        imageIndice.at(i) = swapChain.imageIndex;
    }
    const auto waitSemaphoreCount = a_PresentInfo.waitSemaphores.size();
    std::vector<VkSemaphore> waitSemaphores(waitSemaphoreCount);
    for (auto i = 0u; i < waitSemaphoreCount; ++i) {
        waitSemaphores.at(i) = *a_PresentInfo.waitSemaphores.at(i);
    }
    VkPresentInfoKHR info{ VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
    info.swapchainCount = swapChainCount;
    info.pSwapchains    = swapChains.data();
    info.pImageIndices  = imageIndice.data();
    info.waitSemaphoreCount = waitSemaphoreCount;
    info.pWaitSemaphores    = waitSemaphores.data();
    vkQueuePresentKHR(*a_Queue, &info);
}

Image::Handle GetNextImage(
    const Handle& a_SwapChain,
    const std::chrono::nanoseconds& a_Timeout,
    const Semaphore::Handle& a_Semaphore,
    const Fence::Handle& a_Fence,
    uint32_t& out_ImageIndex)
{
    auto& swapChain = *a_SwapChain;
    vkAcquireNextImageKHR(swapChain.device, swapChain, a_Timeout.count(),
        *a_Semaphore, *a_Fence, &out_ImageIndex);
    swapChain.imageIndex = out_ImageIndex;
    return Image::Handle(swapChain.GetCurrentImage(), [](auto&) {});
}

Image::Handle GetNextImage(
    const Handle& a_SwapChain,
    const std::chrono::nanoseconds& a_Timeout,
    const Semaphore::Handle& a_Semaphore,
    const Fence::Handle& a_Fence)
{
    uint32_t imageIndex = 0;
    return GetNextImage(a_SwapChain, a_Timeout, a_Semaphore, a_Fence, imageIndex);
}

uint32_t GetImageCount(const Handle& a_SwapChain)
{
    return a_SwapChain->images.size();
}
}