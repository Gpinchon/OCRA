#include <VK/SwapChain/Enums.hpp>
#include <VK/SwapChain/Surface.hpp>
#include <VK/SwapChain/SwapChain.hpp>

#include <VK/Device.hpp>
#include <VK/Enums.hpp>
#include <VK/Fence.hpp>
#include <VK/Flags.hpp>
#include <VK/Image.hpp>
#include <VK/Semaphore.hpp>

#include <OCRA/Core.hpp>
#include <OCRA/SwapChain.hpp>

#include <cassert>

namespace OCRA::Device {
SwapChain::Handle CreateSwapChain(
    const Device::Handle& a_Device,
    const CreateSwapChainInfo& a_Info)
{
    auto& device = *a_Device;
    vk::SwapchainCreateInfoKHR info;
    info.clipped               = a_Info.clipped;
    info.compositeAlpha        = vk::CompositeAlphaFlagBitsKHR::eOpaque;
    info.imageArrayLayers      = a_Info.imageArrayLayers;
    info.imageColorSpace       = ConvertToVk(a_Info.imageColorSpace);
    info.imageExtent.height    = a_Info.imageExtent.height;
    info.imageExtent.width     = a_Info.imageExtent.width;
    info.imageFormat           = ConvertToVk(a_Info.imageFormat);
    info.imageSharingMode      = ConvertToVk(a_Info.imageSharingMode);
    info.imageUsage            = ConvertToVk(a_Info.imageUsage);
    info.minImageCount         = a_Info.imageCount;
    info.oldSwapchain          = a_Info.oldSwapchain ? **a_Info.oldSwapchain : nullptr;
    info.queueFamilyIndexCount = a_Info.queueFamilyIndices.size();
    info.pQueueFamilyIndices   = a_Info.queueFamilyIndices.data();
    info.surface               = **a_Info.surface;
    info.preTransform          = vk::SurfaceTransformFlagBitsKHR::eIdentity;
    info.presentMode           = GetVkPresentMode(a_Info.presentMode);
    auto properties            = a_Device->physicalDevice.getImageFormatProperties(info.imageFormat, vk::ImageType::e2D, vk::ImageTiling::eOptimal, info.imageUsage, {});
    auto swapChain             = std::make_shared<SwapChain::Impl>(device, info);
    auto images                = swapChain->getImages();
    swapChain->imagesMemory.resize(images.size());
    swapChain->images.resize(images.size());
    for (auto i = 0u; i < images.size(); ++i) {
        auto& vkImage     = images.at(i);
        auto& image       = swapChain->images.at(i);
        auto& imageMemory = swapChain->imagesMemory.at(i);
        image             = Image::Handle(new (&imageMemory) Image::Impl(device, vkImage), [](auto&) {});
    }
    return swapChain;
}
}

namespace OCRA::SwapChain {
std::pair<Image::Handle, uint32_t> GetNextImage(
    const Handle& a_SwapChain,
    const std::chrono::nanoseconds& a_Timeout,
    const Semaphore::Handle& a_Semaphore,
    const Fence::Handle& a_Fence)
{
    auto& swapChain = *a_SwapChain;
    auto imageIndex = swapChain.acquireNextImage(
                                   a_Timeout.count(),
                                   a_Semaphore != nullptr ? **a_Semaphore : nullptr,
                                   a_Fence != nullptr ? **a_Fence : nullptr)
                          .second;
    swapChain.imageIndex = imageIndex;
    return { swapChain.GetCurrentImage(), imageIndex };
}

uint32_t GetImageCount(const Handle& a_SwapChain)
{
    return a_SwapChain->images.size();
}
}
