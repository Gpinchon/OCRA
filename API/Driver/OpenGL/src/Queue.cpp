#include <OCRA/Handle.hpp>
#include <OCRA/Semaphore.hpp>

#include <GL/Device.hpp>
#include <GL/Queue.hpp>
#include <GL/Fence.hpp>
#include <GL/Semaphore.hpp>
#include <GL/Command/Buffer.hpp>

#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Fence);
OCRA_DECLARE_HANDLE(OCRA::Queue);

namespace OCRA::Queue
{
static inline void Execute(const SubmitInfo& a_SubmitInfo)
{
    for (auto semaphoreIndex = 0u; semaphoreIndex < a_SubmitInfo.waitSemaphores.size(); ++semaphoreIndex)
    {
        auto& semaphore = a_SubmitInfo.waitSemaphores.at(semaphoreIndex);
        if (semaphore->type == Semaphore::Type::Binary)
            std::static_pointer_cast<Semaphore::Binary>(semaphore)->Wait();
        else {
            auto& semaphoreValue = a_SubmitInfo.timelineSemaphoreValues.waitSemaphoreValues.at(semaphoreIndex);
            std::static_pointer_cast<Semaphore::Timeline>(semaphore)->WaitDevice(semaphoreValue);
        }
    }
    OCRA::Command::Buffer::Execute(a_SubmitInfo.commandBuffers);
    for (auto semaphoreIndex = 0u; semaphoreIndex < a_SubmitInfo.signalSemaphores.size(); ++semaphoreIndex)
    {
        auto& semaphore = a_SubmitInfo.signalSemaphores.at(semaphoreIndex);
        if (semaphore->type == Semaphore::Type::Binary)
            std::static_pointer_cast<Semaphore::Binary>(semaphore)->Signal();
        else {
            auto& semaphoreValue = a_SubmitInfo.timelineSemaphoreValues.signalSemaphoreValues.at(semaphoreIndex);
            std::static_pointer_cast<Semaphore::Timeline>(semaphore)->SignalDevice(semaphoreValue);
        }
    }
}

void Submit(
    const Handle& a_Queue,
    const std::vector<SubmitInfo>& a_SubmitInfos,
    const Fence::Handle& a_Fence)
{
    a_Queue->PushCommand([submitInfos = a_SubmitInfos, fence = a_Fence] {
        for (const auto& submitInfo : submitInfos) {
            Execute(submitInfo);
        }
        if (fence != nullptr) fence->Signal();
    }, false);
}
void WaitIdle(const Handle& a_Queue)
{
    a_Queue->WaitIdle();
}
}
