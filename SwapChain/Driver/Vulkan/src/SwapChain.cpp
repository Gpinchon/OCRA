#include <SwapChain.hpp>

#include <VK/Device.hpp>
#include <VK/Enums.hpp>
#include <VK/Flags.hpp>

#include <OCRA/Core.hpp>
#include <OCRA/SwapChain.hpp>

namespace OCRA::Device
{
SwapChain::Handle CreateSwapChain(
    const Device::Handle& a_Device,
    const CreateSwapChainInfo& a_Info)
{
    VkSwapchainKHR swapChain = nullptr;
    VkSwapchainCreateInfoKHR info{ VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
    info.clipped = a_Info.clipped;
    info.imageArrayLayers = a_Info.imageArrayLayers;
    //info.imageColorSpace  = GetVkColorSpace(a_Info.imageColorSpace);
    info.imageExtent.height = a_Info.imageExtent.height;
    info.imageExtent.width  = a_Info.imageExtent.width;
    info.imageFormat        = GetVkFormat(a_Info.imageFormat);
    info.imageSharingMode   = GetVkSharingMode(a_Info.imageSharingMode);
    //info.imageUsage         = GetVkImageUsage(a_Info.imageUsage);
    info.oldSwapchain = a_Info.oldSwapchain ? (VkSwapchainKHR)*a_Info.oldSwapchain : nullptr;
    info.queueFamilyIndexCount = a_Info.queueFamilyIndices.size();
    info.pQueueFamilyIndices   = a_Info.queueFamilyIndices.data();
    vkCreateSwapchainKHR(*a_Device, &info, nullptr, &swapChain);
    return std::make_shared<SwapChain::Impl>(*a_Device, swapChain);
}
}