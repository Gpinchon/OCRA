#include <VK/Device.hpp>
#include <VK/PhysicalDevice.hpp>

#include <OCRA/Structs.hpp>

namespace OCRA::PhysicalDevice
{
Device::Handle CreateDevice(
    const PhysicalDevice::Handle& a_PhysicalDevice,
    const CreateDeviceInfo& a_Info,
    const AllocationCallback* a_Allocator)
{
    const std::vector<const char*> extensions{
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME,
    };
    VkDevice device = nullptr;
    VkDeviceCreateInfo info{ VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
    info.enabledExtensionCount = extensions.size();
    info.ppEnabledExtensionNames = extensions.data();
    std::vector<VkDeviceQueueCreateInfo> vkDeviceQueue;
    vkDeviceQueue.reserve(a_Info.queueInfos.size());
    for (const auto& queueInfo : a_Info.queueInfos) {
        VkDeviceQueueCreateInfo vkQueueInfo{ VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
        vkQueueInfo.pQueuePriorities = queueInfo.queuePriorities.data();
        vkQueueInfo.queueCount = queueInfo.queueCount;
        vkQueueInfo.queueFamilyIndex = queueInfo.queueFamilyIndex;
        vkDeviceQueue.push_back(vkQueueInfo);
    }
    //info.pEnabledFeatures = a_Info.enabledFeatures;
    info.queueCreateInfoCount = vkDeviceQueue.size();
    info.pQueueCreateInfos = vkDeviceQueue.data();
    vkCreateDevice(*a_PhysicalDevice, &info, nullptr, &device);
    return std::make_shared<Device::Impl>(*a_PhysicalDevice, device);
}
}
