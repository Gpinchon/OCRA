#include <VK/Device.hpp>
#include <VK/Fence.hpp>

#include <OCRA/Core.hpp>

#include <chrono>

namespace OCRA::Fence
{
bool WaitFor(
    const std::vector<Fence::Handle>& a_Fences,
    bool a_WaitAll,
    const std::chrono::nanoseconds& a_TimeoutNS)
{
    auto& device = a_Fences.front()->getDevice();
    std::vector<vk::Fence> vkFences;
    vkFences.reserve(a_Fences.size());
    for (const auto& fence : a_Fences)
        vkFences.push_back(**fence);
    return device.waitForFences(vkFences, a_WaitAll, a_TimeoutNS.count()) == vk::Result::eSuccess;
}

bool WaitFor(
    const Fence::Handle& a_Fence,
    const std::chrono::nanoseconds& a_TimeoutNS)
{
    return a_Fence->getDevice().waitForFences({**a_Fence}, true, a_TimeoutNS.count()) == vk::Result::eSuccess;
}

void Reset(const Handle& a_Fence)
{
    auto& fence = *a_Fence;
    fence.getDevice().resetFences({ *fence });
}

void Reset(const std::vector<Handle>& a_Fences)
{
    auto& device = a_Fences.front()->getDevice();
    std::vector<vk::Fence> vkFences;
    vkFences.reserve(a_Fences.size());
    for (const auto& fence : a_Fences)
        vkFences.push_back(**fence);
    device.resetFences(vkFences);
}

FenceStatus GetStatus(const Handle& a_Fence)
{
    auto& fence  = *a_Fence;
    auto& device = fence.getDevice();
    return fence.getDevice().getFenceStatus(*fence) == vk::Result::eSuccess ? FenceStatus::Signaled : FenceStatus::Unsignaled;
}

}