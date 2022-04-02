#include <Handle.hpp>
#include <Queue/Queue.hpp>
#include <Queue/Semaphore.hpp>
#include <Command/Pool.hpp>

#include <GL/Queue/Fence.hpp>
#include <GL/Device.hpp>
#include <GL/Command/Buffer.hpp>
#include <GL/WeakHandle.hpp>
#include <GL/Queue/Semaphore.hpp>

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
	for (const auto& submitInfo : a_SubmitInfos) {
		for (auto semaphoreIndex = 0u; semaphoreIndex < submitInfo.waitSemaphores.size(); ++semaphoreIndex)
		{
			auto& semaphore = submitInfo.waitSemaphores.at(semaphoreIndex);
			if (Semaphore::GetType(semaphore) == Semaphore::Type::Binary)
				Semaphore::WaitDevice(semaphore);
			else {
				auto& semaphoreValue = submitInfo.timelineSemaphoreValues.waitSemaphoreValues.at(semaphoreIndex);
				Semaphore::WaitDevice(semaphore, semaphoreValue);
			}
		}
		Command::Buffer::Execute(submitInfo.commandBuffers);
		for (auto semaphoreIndex = 0u; semaphoreIndex < submitInfo.signalSemaphores.size(); ++semaphoreIndex)
		{
			auto& semaphore = submitInfo.signalSemaphores.at(semaphoreIndex);
			if (Semaphore::GetType(semaphore) == Semaphore::Type::Binary)
				Semaphore::WaitDevice(semaphore);
			else {
				auto& semaphoreValue = submitInfo.timelineSemaphoreValues.signalSemaphoreValues.at(semaphoreIndex);
				Semaphore::WaitDevice(semaphore, semaphoreValue);
			}
		}
	}
	if (a_Fence != nullptr) Fence::Signal(device, { a_Fence });
}
}