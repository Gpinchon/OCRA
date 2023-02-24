#include <VK/Device.hpp>
#include <VK/PhysicalDevice.hpp>
#include <VK/Queue.hpp>
#include <VK/Memory.hpp>

#include <OCRA/Structs.hpp>

namespace OCRA::PhysicalDevice
{
Device::Handle CreateDevice(
    const PhysicalDevice::Handle& a_PhysicalDevice,
    const CreateDeviceInfo& a_Info,
    const AllocationCallback* a_Allocator)
{
    VkDeviceCreateInfo info{ VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
    //info.pEnabledFeatures = a_Info.enabledFeatures;
    VkDevice device{};
    vkCreateDevice(*a_PhysicalDevice, &info, nullptr, &device);
    return std::make_shared<Device::Impl>(device);
}
}

namespace OCRA::Device {
Queue::Handle GetQueue(
    const Handle& a_Device,
    uint32_t a_FamilyIndex,
    uint32_t a_QueueIndex)
{
    VkQueue queue{};
    vkGetDeviceQueue(*a_Device, a_FamilyIndex, a_QueueIndex, &queue);
    return std::make_shared<Queue::Impl>(queue);
}

Memory::Handle AllocateMemory(const Device::Handle& a_Device, const AllocateMemoryInfo& a_Info)
{
    VkDeviceMemory memory = nullptr;
    VkMemoryAllocateInfo info{ VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
    info.allocationSize = a_Info.size;
    info.memoryTypeIndex = a_Info.memoryTypeIndex;
    vkAllocateMemory(*a_Device, &info, nullptr, &memory);
    return std::make_shared<Memory::Impl>(*a_Device, memory);
}

}