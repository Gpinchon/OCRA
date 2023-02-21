#include <OCRA/Semaphore.hpp>

#include <GL/Common/Assert.hpp>
#include <GL/Semaphore.hpp>

#include <stdexcept>

namespace OCRA {
struct AllocationCallback;
}

namespace OCRA::Semaphore {
Handle Create(
    const Device::Handle& a_Device,
    const Info& a_Info,
    const AllocationCallback* a_Allocator)
{
    if (a_Info.type == Type::Timeline)
        return Handle(new Timeline(a_Info.initialValue));
    else if (a_Info.type == Type::Binary)
        return Handle(new Binary);
    throw std::runtime_error("Unknown semaphore type");
}
void Signal(
    const Device::Handle& a_Device,
    const Handle& a_Semaphore,
    const uint64_t& a_Value)
{
    OCRA_ASSERT(a_Semaphore->type == Type::Timeline);
    std::static_pointer_cast<Timeline>(a_Semaphore)->SignalClient(a_Value);
}
void Signal(
    const Device::Handle& a_Device,
    const std::vector<Handle>& a_Semaphores,
    const std::vector<uint64_t>& a_Values)
{
    for (auto semaphoreIndex = 0u; semaphoreIndex < a_Semaphores.size(); ++semaphoreIndex)
        Signal(a_Device, a_Semaphores.at(semaphoreIndex), a_Values.at(semaphoreIndex));
}
bool Wait(
    const Device::Handle& a_Device,
    const Handle& a_Semaphore,
    const uint64_t& a_Value,
    const std::chrono::nanoseconds& a_TimeoutNS)
{
    OCRA_ASSERT(a_Semaphore->type == Type::Timeline);
    return std::static_pointer_cast<Timeline>(a_Semaphore)->WaitClient(a_TimeoutNS, a_Value);
}
bool Wait(
    const Device::Handle& a_Device,
    const std::vector<Handle>& a_Semaphores,
    const std::vector<uint64_t>& a_Values,
    const std::chrono::nanoseconds& a_TimeoutNS)
{
    std::chrono::nanoseconds timeout = a_TimeoutNS; //make a copy to edit it
    const auto waitStart = std::chrono::system_clock::now();
    for (auto i = 0u; i < a_Semaphores.size(); ++i) {
        const auto waitResult = Wait(a_Device, a_Semaphores.at(i), a_Values.at(i), timeout);
        if (!waitResult) return false;
        const auto waitDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - waitStart);
        timeout -= waitDuration;
    }
    return true;
}
uint64_t GetCounterValue(
    const Device::Handle& a_Device,
    const Handle& a_Semaphore)
{
    OCRA_ASSERT(a_Semaphore->type == Type::Timeline);
    return std::static_pointer_cast<Timeline>(a_Semaphore)->GetCount();
}
}