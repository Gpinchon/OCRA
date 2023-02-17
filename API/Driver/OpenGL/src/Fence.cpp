#include <OCRA/Allocator.hpp>

#include <GL/Fence.hpp>

namespace OCRA::Fence {
Handle Create(
    const Device::Handle& a_Device,
    const AllocationCallback* a_Allocator)
{
    if (a_Allocator != nullptr) {
        auto ptr = a_Allocator->allocationFunc(
            a_Allocator->userData,
            sizeof(Impl), alignof(Impl),
            AllocationScope::Object);
        return Handle(new(ptr) Impl(a_Device), Deleter(a_Allocator->userData, a_Allocator->freeFunc));
    }
    else return Handle(new Impl(a_Device));
}
bool WaitFor(
    const Device::Handle& a_Device,
    const Handle& a_Fences,
    const std::chrono::nanoseconds& a_TimeoutNS)
{
    return a_Fences->WaitFor(a_TimeoutNS);
}
bool WaitFor(
    const Device::Handle& a_Device,
    const std::vector<Handle>& a_Fences,
    bool a_WaitAll,
    const std::chrono::nanoseconds& a_TimeoutNS)
{
    bool ret = false;
    for (auto& fence : a_Fences) {
        const auto waitResult = fence->WaitFor(a_TimeoutNS);
        if (a_WaitAll) ret |= waitResult;
        else return waitResult;
    }
    return ret;
}
void Reset(
    const Device::Handle& a_Device,
    const std::vector<Handle>& a_Fences)
{
    for (auto& fence : a_Fences) fence->Reset();
}
Status GetStatus(
    const Device::Handle& a_Device,
    const Handle& a_Fence)
{
    return a_Fence->GetStatus();
}
}