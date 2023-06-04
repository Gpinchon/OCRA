#include <OCRA/Core.hpp>

#include <VK/CommandBuffer.hpp>
#include <VK/Device.hpp>
#include <VK/Fence.hpp>
#include <VK/Flags.hpp>
#include <VK/Queue.hpp>
#include <VK/Semaphore.hpp>

namespace OCRA::Queue {
struct TransitoryQueueSubmitInfo {
    std::vector<vk::CommandBuffer> vkCommandBuffers;
    std::vector<vk::Semaphore> vkSignSemaphores;
    std::vector<uint64_t> vkSignValue;
    std::vector<vk::Semaphore> vkWaitSemaphores;
    std::vector<uint64_t> vkWaitValue;
    std::vector<vk::PipelineStageFlags> vkWaitDstStages;
};
void Submit(
    const Handle& a_Queue,
    const std::vector<QueueSubmitInfo>& a_SubmitInfos,
    const Fence::Handle& a_Fence)
{
    std::vector<vk::SubmitInfo> vkSubmitInfos(a_SubmitInfos.size());
    std::vector<vk::TimelineSemaphoreSubmitInfo> vkTimelineSemaphoreInfos(a_SubmitInfos.size());
    std::vector<TransitoryQueueSubmitInfo> tSubmitInfos(a_SubmitInfos.size());

    for (auto i = 0u; i < a_SubmitInfos.size(); ++i) {
        auto& submitInfo              = a_SubmitInfos.at(i);
        auto& tSubmitInfo             = tSubmitInfos.at(i);
        auto& vkSubmitInfo            = vkSubmitInfos.at(i);
        auto& vkCommandBuffers        = tSubmitInfo.vkCommandBuffers;
        auto& vkSignSemaphores        = tSubmitInfo.vkSignSemaphores;
        auto& vkSignValues            = tSubmitInfo.vkSignValue;
        auto& vkWaitSemaphores        = tSubmitInfo.vkWaitSemaphores;
        auto& vkWaitValues            = tSubmitInfo.vkWaitValue;
        auto& vkWaitDstFlags          = tSubmitInfo.vkWaitDstStages;
        auto& vkTimelineSemaphoreInfo = vkTimelineSemaphoreInfos.at(i);

        vkSubmitInfo.pNext = &vkTimelineSemaphoreInfo;

        vkCommandBuffers.reserve(submitInfo.commandBuffers.size());
        for (const auto& handle : submitInfo.commandBuffers) {
            vkCommandBuffers.push_back(**handle);
        }
        vkSubmitInfo.commandBufferCount = vkCommandBuffers.size();
        vkSubmitInfo.pCommandBuffers    = vkCommandBuffers.data();

        vkWaitSemaphores.reserve(submitInfo.waitSemaphores.size());
        for (const auto& waitInfo : submitInfo.waitSemaphores) {
            vkWaitSemaphores.push_back(**waitInfo.semaphore);
            vkWaitValues.push_back(waitInfo.timelineValue);
            vkWaitDstFlags.push_back(ConvertToVk(waitInfo.dstStages));
        }
        vkSubmitInfo.waitSemaphoreCount                 = vkWaitSemaphores.size();
        vkSubmitInfo.pWaitSemaphores                    = vkWaitSemaphores.data();
        vkSubmitInfo.pWaitDstStageMask                  = vkWaitDstFlags.data();
        vkTimelineSemaphoreInfo.waitSemaphoreValueCount = vkWaitValues.size();
        vkTimelineSemaphoreInfo.pWaitSemaphoreValues    = vkWaitValues.data();

        vkSignSemaphores.reserve(submitInfo.waitSemaphores.size());
        for (const auto& signInfo : submitInfo.signalSemaphores) {
            vkSignSemaphores.push_back(**signInfo.semaphore);
            vkSignValues.push_back(signInfo.timelineValue);
        }
        vkSubmitInfo.signalSemaphoreCount                 = vkSignSemaphores.size();
        vkSubmitInfo.pSignalSemaphores                    = vkSignSemaphores.data();
        vkTimelineSemaphoreInfo.signalSemaphoreValueCount = vkSignValues.size();
        vkTimelineSemaphoreInfo.pSignalSemaphoreValues    = vkSignValues.data();
    }
    a_Queue->submit(vkSubmitInfos, a_Fence ? **a_Fence : VK_NULL_HANDLE);
}
void WaitIdle(const Handle& a_Queue)
{
    a_Queue->waitIdle();
}
}
