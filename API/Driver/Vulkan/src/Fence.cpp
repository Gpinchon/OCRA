#include <VK/Device.hpp>
#include <VK/Fence.hpp>

#include <OCRA/Fence.hpp>

namespace OCRA::Fence
{
Handle Create(
    const Device::Handle& a_Device,
    const Status& a_DefaultStatus,
    const AllocationCallback* a_Allocator)
{
    VkFence fence{};
    VkFenceCreateInfo info{ VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
    info.flags = a_DefaultStatus == Status::Signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;
    vkCreateFence(*a_Device, &info, nullptr, &fence);
    return std::make_shared<Impl>(*a_Device, fence);
}

bool WaitFor(
    const std::vector<Handle>& a_Fences,
    bool a_WaitAll,
    const std::chrono::nanoseconds& a_TimeoutNS)
{
    const auto& device = a_Fences.front()->device;
    std::vector<VkFence> vkFences;
    vkFences.reserve(a_Fences.size());
    for (const auto& fence : a_Fences)
        vkFences.push_back(*fence);
    return vkWaitForFences(device, vkFences.size(), vkFences.data(), a_WaitAll, a_TimeoutNS.count()) == VK_SUCCESS;
}

bool WaitFor(
    const Handle& a_Fences,
    const std::chrono::nanoseconds& a_TimeoutNS)
{
    const VkFence& vkFence = *a_Fences;
    return vkWaitForFences(a_Fences->device, 1, &vkFence, true, a_TimeoutNS.count()) == VK_SUCCESS;
}

void Reset(const std::vector<Handle>& a_Fences)
{
    const auto& device = a_Fences.front()->device;
    std::vector<VkFence> vkFences;
    vkFences.reserve(a_Fences.size());
    for (const auto& fence : a_Fences)
        vkFences.push_back(*fence);
    vkResetFences(device, vkFences.size(), vkFences.data());
}

Status GetStatus(const Handle& a_Fence)
{
    const auto& fence = *a_Fence;
    return vkGetFenceStatus(fence.device, fence) == VK_SUCCESS ? Status::Signaled : Status::Unsignaled;
}

}