#include <Device.hpp>
#include <PhysicalDevice.hpp>

#include <GL/Device.hpp>
#include <GL/Queue/Queue.hpp>

namespace OCRA::Device
{
Impl::Impl(const PhysicalDevice::Handle& a_PhysicalDevice, const Info& a_Info)
	: info(a_Info)
	, physicalDevice(a_PhysicalDevice)
{}
Handle Create(const PhysicalDevice::Handle& a_PhysicalDevice, const Info& a_Info, const AllocationCallback* a_Allocator)
{
	auto device = Handle(new Impl(a_PhysicalDevice, a_Info));
	for (const auto& queueInfo : a_Info.queueInfos)
	{
		for (auto queueIndex = 0u; queueIndex < queueInfo.queueCount; ++queueIndex)
			device->queueFamilies[queueInfo.queueFamilyIndex].push_back(Queue::Handle(new Queue::Impl(
				device,
				queueInfo.queueFamilyIndex,
				queueIndex)));
	}
	return device;
}
Queue::Handle GetQueue(const Handle& a_Device, uint32_t a_FamilyIndex, uint32_t a_QueueIndex) {
	return a_Device->queueFamilies.at(a_FamilyIndex).at(a_QueueIndex);
}
}