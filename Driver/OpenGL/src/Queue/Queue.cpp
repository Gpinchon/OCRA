#include <Handle.hpp>
#include <Queue/Queue.hpp>
#include <Queue/Semaphore.hpp>
#include <Command/Pool.hpp>

#include <GL/Queue/Queue.hpp>
#include <GL/Queue/Fence.hpp>
#include <GL/Device.hpp>
#include <GL/Command/Buffer.hpp>
#include <GL/WeakHandle.hpp>
#include <GL/Queue/Semaphore.hpp>

#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Fence);
OCRA_DECLARE_HANDLE(OCRA::Queue);

namespace OCRA::Queue
{
static inline void Execute(
	const Handle& a_Queue,
	const std::vector<SubmitInfo>& a_SubmitInfos,
	const Fence::Handle& a_Fence)
{
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
	if (a_Fence != nullptr) a_Fence->Signal();
}

void Submit(
	const Handle& a_Queue,
	const std::vector<SubmitInfo>& a_SubmitInfos,
	const Fence::Handle& a_Fence)
{
	a_Queue->device.lock()->PushCommand(a_Queue->familyIndex, a_Queue->queueIndex,
	[queue = a_Queue, submitInfos = a_SubmitInfos, fence = a_Fence] {
		Execute(queue, submitInfos, fence);
	}, false);
}
}