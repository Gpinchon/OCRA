#include <GL/Device.hpp>
#include <GL/Queue.hpp>

#include <OCRA/Core.hpp>

namespace OCRA::PhysicalDevice
{
Device::Handle CreateDevice(
    const PhysicalDevice::Handle& a_PhysicalDevice,
    const CreateDeviceInfo& a_Info,
    const AllocationCallback* a_Allocator)
{
    const auto device = std::make_shared<Device::Impl>(a_PhysicalDevice);
    for (const auto& queueInfo : a_Info.queueInfos)
    {
        for (auto queueIndex = 0u; queueIndex < queueInfo.queueCount; ++queueIndex)
            device->queueFamilies[queueInfo.queueFamilyIndex].push_back(std::make_shared<OCRA::Queue::Impl>(device, queueInfo.queueFamilyIndex, queueIndex));
    }
    return device;
}
}


namespace OCRA::Device
{
Impl::Impl(const PhysicalDevice::Handle& a_PhysicalDevice)
    : physicalDevice(a_PhysicalDevice)
{}

Queue::Handle GetQueue(const Handle& a_Device, uint32_t a_FamilyIndex, uint32_t a_QueueIndex) {
    return a_Device->queueFamilies.at(a_FamilyIndex).at(a_QueueIndex);
}
void WaitIdle(const Handle& a_Device)
{
    for (const auto& queueFamily : a_Device->queueFamilies) {
        for (const auto& queue : queueFamily.second)
            queue->WaitIdle();
    }
    
}
}