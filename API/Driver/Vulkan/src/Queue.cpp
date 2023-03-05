#include <VK/CommandBuffer.hpp>
#include <VK/Device.hpp>
#include <VK/Fence.hpp>
#include <VK/Queue.hpp>
#include <VK/Semaphore.hpp>
#include <VK/Flags.hpp>

#include <OCRA/Structs.hpp>

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
}

namespace OCRA::Queue
{
struct TransitoryQueueSubmitInfo {
    std::vector<VkCommandBuffer>      vkCommandBuffers;
    std::vector<VkSemaphore>          vkSignSemaphores;
    std::vector<uint64_t>             vkSignValue;
    std::vector<VkSemaphore>          vkWaitSemaphores;
    std::vector<uint64_t>             vkWaitValue;
    std::vector<VkPipelineStageFlags> vkWaitDstStages;
};
void Submit(
    const Handle& a_Queue,
    const std::vector<QueueSubmitInfo>& a_SubmitInfos,
    const Fence::Handle& a_Fence)
{
    std::vector<VkSubmitInfo> vkSubmitInfos(a_SubmitInfos.size(), { VK_STRUCTURE_TYPE_SUBMIT_INFO });
    std::vector<VkTimelineSemaphoreSubmitInfo> vkTimelineSemaphoreInfos(a_SubmitInfos.size(), { VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO });
    std::vector<TransitoryQueueSubmitInfo> tSubmitInfos(a_SubmitInfos.size());

    for (auto i = 0u; i < a_SubmitInfos.size(); ++i) {
        auto& submitInfo  = a_SubmitInfos.at(i);
        auto& tSubmitInfo = tSubmitInfos.at(i);
        auto& vkSubmitInfo = vkSubmitInfos.at(i);
        auto& vkCommandBuffers = tSubmitInfo.vkCommandBuffers;
        auto& vkSignSemaphores = tSubmitInfo.vkSignSemaphores;
        auto& vkSignValues     = tSubmitInfo.vkSignValue;
        auto& vkWaitSemaphores = tSubmitInfo.vkWaitSemaphores;
        auto& vkWaitValues     = tSubmitInfo.vkWaitValue;
        auto& vkWaitDstFlags   = tSubmitInfo.vkWaitDstStages;
        auto& vkTimelineSemaphoreInfo = vkTimelineSemaphoreInfos.at(i);

        vkSubmitInfo.pNext = &vkTimelineSemaphoreInfo;

        vkCommandBuffers.reserve(submitInfo.commandBuffers.size());
        for (const auto& handle : submitInfo.commandBuffers) {
            vkCommandBuffers.push_back(*handle);
        }
        vkSubmitInfo.commandBufferCount = vkCommandBuffers.size();
        vkSubmitInfo.pCommandBuffers    = vkCommandBuffers.data();

        vkWaitSemaphores.reserve(submitInfo.waitSemaphores.size());
        for (const auto& waitInfo : submitInfo.waitSemaphores) {
            vkWaitSemaphores.push_back(*waitInfo.semaphore);
            vkWaitValues.push_back(waitInfo.timelineValue);
            vkWaitDstFlags.push_back(GetVkPipelineStageFlags(waitInfo.dstStages));
        }
        vkSubmitInfo.waitSemaphoreCount = vkWaitSemaphores.size();
        vkSubmitInfo.pWaitSemaphores    = vkWaitSemaphores.data();
        vkSubmitInfo.pWaitDstStageMask  = vkWaitDstFlags.data();
        vkTimelineSemaphoreInfo.waitSemaphoreValueCount = vkWaitValues.size();
        vkTimelineSemaphoreInfo.pWaitSemaphoreValues    = vkWaitValues.data();

        vkSignSemaphores.reserve(submitInfo.waitSemaphores.size());
        for (const auto& signInfo : submitInfo.signalSemaphores) {
            vkSignSemaphores.push_back(*signInfo.semaphore);
            vkSignValues.push_back(signInfo.timelineValue);
        }
        vkSubmitInfo.signalSemaphoreCount = vkSignSemaphores.size();
        vkSubmitInfo.pSignalSemaphores    = vkSignSemaphores.data();
        vkTimelineSemaphoreInfo.signalSemaphoreValueCount = vkSignValues.size();
        vkTimelineSemaphoreInfo.pSignalSemaphoreValues    = vkSignValues.data();
    }
    vkQueueSubmit(*a_Queue, vkSubmitInfos.size(), vkSubmitInfos.data(), a_Fence ? (VkFence)*a_Fence : VK_NULL_HANDLE);
}
void WaitIdle(const Handle& a_Queue)
{
    vkQueueWaitIdle(*a_Queue);
}
}