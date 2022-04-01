#include <Handle.hpp>
#include <Queue/Queue.hpp>
#include <Queue/Semaphore.hpp>

#include <GL/Queue/Fence.hpp>
#include <GL/Device.hpp>
#include <GL/PhysicalDevice.hpp>
#include <GL/Command/Buffer.hpp>
#include <GL/WeakHandle.hpp>

#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Fence);
OCRA_DECLARE_HANDLE(OCRA::Queue);
OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Queue
{
struct Impl
{
	Impl(const Device::Handle& a_PhysicalDevice,
		const uint32_t& a_FamilyIndex,
		const uint32_t& a_QueueIndex)
		: device(a_PhysicalDevice)
		, familyIndex(a_FamilyIndex)
		, queueIndex(a_QueueIndex)
	{}
	const Device::WeakHandle device;
	const uint32_t familyIndex;
	const uint32_t queueIndex;
};
Handle Create(
	const Device::Handle& a_PhysicalDevice,
	const uint32_t& a_FamilyIndex,
	const uint32_t& a_QueueIndex)
{
	return Handle(new Impl(a_PhysicalDevice, a_FamilyIndex, a_QueueIndex));
}

void Submit(
	const Handle& a_Queue,
	const std::vector<SubmitInfo>& a_SubmitInfos,
	const Fence::Handle& a_Fence)
{
	const auto device = a_Queue->device.lock();
	const auto physicalDevice = Device::GetPhysicalDevice(device);
	PhysicalDevice::PushCommand(physicalDevice, a_Queue->familyIndex, a_Queue->queueIndex, [device = device, submitInfos = a_SubmitInfos, fence = a_Fence] {
		for (const auto& submitInfo : submitInfos) {
			{
				std::vector<uint64_t> waitSemaphoreValues(submitInfo.waitSemaphores.size(), 1);
				Semaphore::Wait(device, submitInfo.waitSemaphores, waitSemaphoreValues, std::numeric_limits<uint64_t>::max());
			}
			Command::Buffer::Submit(submitInfo.commandBuffers);
			{
				std::vector<uint64_t> signalSemaphoreValues(submitInfo.waitSemaphores.size(), 1);
				Semaphore::Signal(device, submitInfo.signalSemaphores, signalSemaphoreValues);
			}
		}
		if (fence != nullptr) Fence::Signal(device, { fence });
	});
}
}