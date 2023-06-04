#include <OCRA/Allocator.hpp>

#include <GL/Fence.hpp>

namespace OCRA::Device {
Fence::Handle CreateFence(
    const Device::Handle& a_Device,
    const FenceStatus& a_DefaultStatus,
    const AllocationCallback* a_Allocator)
{
    if (a_Allocator != nullptr) {
        auto ptr = a_Allocator->allocationFunc(
            a_Allocator->userData,
            sizeof(Impl), alignof(Impl),
            AllocationScope::Object);
        return Fence::Handle(new (ptr) Fence::Impl(a_DefaultStatus), Deleter(a_Allocator->userData, a_Allocator->freeFunc));
    } else
        return std::make_shared<Fence::Impl>(a_DefaultStatus);
}
}

namespace OCRA::Fence {
bool WaitFor(
    const Handle& a_Fence,
    const std::chrono::nanoseconds& a_TimeoutNS)
{
    return a_Fence->WaitFor(a_TimeoutNS);
}

bool WaitFor(
    const std::vector<Handle>& a_Fences,
    bool a_WaitAll,
    const std::chrono::nanoseconds& a_TimeoutNS)
{
    bool ret = false;
    for (auto& fence : a_Fences) {
        const auto waitResult = WaitFor(fence, a_TimeoutNS);
        if (a_WaitAll)
            ret |= waitResult;
        else
            return waitResult;
    }
    return ret;
}

void Reset(const Handle& a_Fence)
{
    a_Fence->Reset();
}

void Reset(const std::vector<Handle>& a_Fences)
{
    for (auto& fence : a_Fences)
        fence->Reset();
}

FenceStatus GetStatus(const Handle& a_Fence)
{
    return a_Fence->GetStatus();
}
}
