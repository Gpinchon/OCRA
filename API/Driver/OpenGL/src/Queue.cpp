#include <OCRA/Core.hpp>

#include <GL/CommandBuffer.hpp>
#include <GL/Device.hpp>
#include <GL/Fence.hpp>
#include <GL/Queue.hpp>
#include <GL/Semaphore.hpp>

#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Fence);
OCRA_DECLARE_HANDLE(OCRA::Queue);

namespace OCRA::Queue {
static inline void Execute(const QueueSubmitInfo& a_SubmitInfo)
{
    for (auto& waitInfo : a_SubmitInfo.waitSemaphores) {
        if (waitInfo.semaphore->type == SemaphoreType::Binary)
            std::static_pointer_cast<Semaphore::Binary>(waitInfo.semaphore)->Wait();
        else {
            auto& semaphoreValue = waitInfo.timelineValue;
            std::static_pointer_cast<Semaphore::Timeline>(waitInfo.semaphore)->WaitDevice(semaphoreValue);
        }
    }
    OCRA::Command::Buffer::Execute(a_SubmitInfo.commandBuffers);
    for (auto& signalInfo : a_SubmitInfo.signalSemaphores) {
        if (signalInfo.semaphore->type == SemaphoreType::Binary)
            std::static_pointer_cast<Semaphore::Binary>(signalInfo.semaphore)->Signal();
        else {
            auto& semaphoreValue = signalInfo.timelineValue;
            std::static_pointer_cast<Semaphore::Timeline>(signalInfo.semaphore)->SignalDevice(semaphoreValue);
        }
    }
}

void Submit(
    const Handle& a_Queue,
    const std::vector<QueueSubmitInfo>& a_SubmitInfos,
    const Fence::Handle& a_Fence)
{
    a_Queue->PushCommand([submitInfos = a_SubmitInfos, fence = a_Fence] {
        for (const auto& submitInfo : submitInfos) {
            Execute(submitInfo);
        }
        if (fence != nullptr)
            fence->Signal();
    },
        false);
}
void WaitIdle(const Handle& a_Queue)
{
    a_Queue->WaitIdle();
}
}
