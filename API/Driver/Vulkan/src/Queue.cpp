#include <VK/CommandBuffer.hpp>
#include <VK/Device.hpp>
#include <VK/Fence.hpp>
#include <VK/Queue.hpp>
#include <VK/Semaphore.hpp>

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
void Submit(
    const Handle& a_Queue,
    const std::vector<QueueSubmitInfo>& a_SubmitInfos,
    const Fence::Handle& a_Fence)
{
    std::vector<VkSubmitInfo> submitInfos;
    std::vector<std::vector<VkCommandBuffer>> vkCommandBuffers;
    std::vector<std::vector<VkSemaphore>>     vkWaitSemaphores;
    std::vector<std::vector<VkSemaphore>>     vkSignSemaphores;

    submitInfos.reserve(a_SubmitInfos.size());
    vkCommandBuffers.resize(a_SubmitInfos.size());
    vkWaitSemaphores.resize(a_SubmitInfos.size());
    vkSignSemaphores.resize(a_SubmitInfos.size());

    for (auto i = 0u; i < a_SubmitInfos.size(); ++i) {
        const auto& submitInfo = a_SubmitInfos.at(i);
        VkSubmitInfo vkSubmitInfo{ VK_STRUCTURE_TYPE_SUBMIT_INFO };

        vkCommandBuffers.at(i).reserve(submitInfo.commandBuffers.size());
        for (const auto& handle : submitInfo.commandBuffers) {
            vkCommandBuffers.at(i).push_back(*handle);
        }
        vkSubmitInfo.commandBufferCount = vkCommandBuffers.at(i).size();
        vkSubmitInfo.pCommandBuffers    = vkCommandBuffers.at(i).data();

        vkWaitSemaphores.at(i).reserve(submitInfo.waitSemaphores.size());
        for (const auto& handle : submitInfo.waitSemaphores) {
            vkWaitSemaphores.at(i).push_back(*handle);
        }
        vkSubmitInfo.waitSemaphoreCount = vkWaitSemaphores.at(i).size();
        vkSubmitInfo.pWaitSemaphores    = vkWaitSemaphores.at(i).data();

        vkSignSemaphores.at(i).reserve(submitInfo.waitSemaphores.size());
        for (const auto& handle : submitInfo.signalSemaphores) {
            vkSignSemaphores.at(i).push_back(*handle);
        }
        vkSubmitInfo.signalSemaphoreCount = vkSignSemaphores.at(i).size();
        vkSubmitInfo.pSignalSemaphores = vkSignSemaphores.at(i).data();
        
        submitInfos.push_back(vkSubmitInfo);
    }
    vkQueueSubmit(*a_Queue, submitInfos.size(), submitInfos.data(), *a_Fence);
}
void WaitIdle(const Handle& a_Queue)
{
    vkQueueWaitIdle(*a_Queue);
}
}