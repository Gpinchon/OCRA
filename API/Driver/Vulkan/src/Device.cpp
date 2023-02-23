#include <VK/Device.hpp>
#include <VK/PhysicalDevice.hpp>
#include <VK/Queue.hpp>

#include <OCRA/Device.hpp>

namespace OCRA::Device {
auto CreateDevice(const VkPhysicalDevice& a_PhysicalDevice, const Info& a_Info) {
    VkDeviceCreateInfo info{ VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
    //info.pEnabledFeatures = a_Info.enabledFeatures;
    VkDevice device{};
    vkCreateDevice(a_PhysicalDevice, &info, nullptr, &device);
    return device;
}

Impl::Impl(const VkPhysicalDevice& a_PhysicalDevice, const Info& a_Info)
    : device(CreateDevice(a_PhysicalDevice, a_Info))
{}

Handle Create(
    const PhysicalDevice::Handle& a_PhysicalDevice,
    const Info& a_Info,
    const AllocationCallback* a_Allocator)
{
    return std::make_shared<Impl>(*a_PhysicalDevice, a_Info);
}

Queue::Handle GetQueue(
    const Handle& a_Device,
    uint32_t a_FamilyIndex,
    uint32_t a_QueueIndex)
{
    VkQueue queue{};
    vkGetDeviceQueue(*a_Device, a_FamilyIndex, a_QueueIndex, &queue);
    return std::make_shared<Queue::Impl>(queue);
}

}