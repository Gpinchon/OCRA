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
    vk::SwapchainCreateInfoKHR info;
    info.clipped = a_Info.clipped;
    info.compositeAlpha     = vk::CompositeAlphaFlagBitsKHR::eOpaque;
    info.imageArrayLayers   = a_Info.imageArrayLayers;
    info.imageColorSpace    = GetVkColorSpace(a_Info.imageColorSpace);
    info.imageExtent.height = a_Info.imageExtent.height;
    info.imageExtent.width  = a_Info.imageExtent.width;
    info.imageFormat        = GetVkFormat(a_Info.imageFormat);
    info.imageSharingMode   = GetVkSharingMode(a_Info.imageSharingMode);
    info.imageUsage         = GetVkImageUsage(a_Info.imageUsage);
    info.minImageCount = a_Info.imageCount;
    info.oldSwapchain = a_Info.oldSwapchain ? **a_Info.oldSwapchain : nullptr;
    info.queueFamilyIndexCount = a_Info.queueFamilyIndices.size();
    info.pQueueFamilyIndices   = a_Info.queueFamilyIndices.data();
    info.surface = **a_Info.surface;
    info.preTransform = vk::SurfaceTransformFlagBitsKHR::eIdentity;
    info.presentMode = GetVkPresentMode(a_Info.presentMode);
    auto properties = a_Device->physicalDevice.getImageFormatProperties(info.imageFormat,
        vk::ImageType::e2D, vk::ImageTiling::eOptimal, info.imageUsage, {});
    auto swapChain = std::make_shared<SwapChain::Impl>(device, info);
    auto images = swapChain->getImages();
    swapChain->imagesMemory.resize(images.size());
    swapChain->images.resize(images.size());
    for (auto i = 0u; i < images.size(); ++i) {
        auto& vkImage     = images.at(i);
        auto& image       = swapChain->images.at(i);
        auto& imageMemory = swapChain->imagesMemory.at(i);
        image = Image::Handle(new(&imageMemory) Image::Impl(device, vkImage), [](auto&){});
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
    std::vector<vk::SwapchainKHR> swapChains(swapChainCount);
    std::vector<uint32_t> imageIndice(swapChainCount);
    for (auto i = 0u; i < swapChainCount; ++i) {
        const auto& swapChain = *a_PresentInfo.swapChains.at(i);
        swapChains.at(i)  = *swapChain;
        imageIndice.at(i) = swapChain.imageIndex;
    }
    const auto waitSemaphoreCount = a_PresentInfo.waitSemaphores.size();
    std::vector<vk::Semaphore> waitSemaphores(waitSemaphoreCount);
    for (auto i = 0u; i < waitSemaphoreCount; ++i) {
        waitSemaphores.at(i) = **a_PresentInfo.waitSemaphores.at(i);
    }
    vk::PresentInfoKHR info{ VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
    info.swapchainCount = swapChainCount;
    info.pSwapchains    = swapChains.data();
    info.pImageIndices  = imageIndice.data();
    info.waitSemaphoreCount = waitSemaphoreCount;
    info.pWaitSemaphores    = waitSemaphores.data();
    a_Queue->presentKHR(info);
}

std::pair<Image::Handle, uint32_t> GetNextImage(
    const Handle& a_SwapChain,
    const std::chrono::nanoseconds& a_Timeout,
    const Semaphore::Handle& a_Semaphore,
    const Fence::Handle& a_Fence)
{
    auto& swapChain = *a_SwapChain;
    auto imageIndex = swapChain.acquireNextImage(a_Timeout.count(),
        **a_Semaphore, **a_Fence).second;
    swapChain.imageIndex = imageIndex;
    return { swapChain.GetCurrentImage(), imageIndex };
}

uint32_t GetImageCount(const Handle& a_SwapChain)
{
    return a_SwapChain->images.size();
}
}