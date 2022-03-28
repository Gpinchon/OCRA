#include <Device.hpp>
#include <PhysicalDevice.hpp>

#include <GL/Queue/Queue.hpp>

#include <vector>

namespace OCRA::Device
{
struct Impl
{
	Impl(const PhysicalDevice::Handle& a_PhysicalDevice, const Info& a_Info)
		: info(a_Info)
		, physicalDevice(a_PhysicalDevice)
	{
		queues.resize(PhysicalDevice::GetQueueFamilyProperties(physicalDevice).size());
		for (const auto& queueInfo : info.queueInfos)
		{
			auto& queueFamily = queues.at(queueInfo.queueFamilyIndex);
			for (auto i = 0u; i < queueInfo.queueCount; ++i)
				queueFamily.push_back(Queue::Handle(new Queue::Impl));
		}
	}
	const Info info;
	const PhysicalDevice::Handle physicalDevice;
	std::vector<std::vector<Queue::Handle>> queues;
};
Handle Create(const PhysicalDevice::Handle& a_PhysicalDevice, const Info& a_Info, const AllocationCallback* a_Allocator)
{
	return Handle(new Impl(a_PhysicalDevice, a_Info));
}
Queue::Handle GetQueue(const Handle& a_Device, uint32_t a_FamilyIndex, uint32_t a_QueueIndex)
{
	return a_Device->queues.at(a_FamilyIndex).at(a_QueueIndex);
}
}