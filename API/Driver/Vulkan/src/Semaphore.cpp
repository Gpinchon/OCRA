#include <OCRA/Structs.hpp>

#include <VK/Device.hpp>
#include <VK/Semaphore.hpp>

#include <chrono>

namespace OCRA::Semaphore {
void Signal(
    const Handle& a_Semaphore,
    const uint64_t& a_Value)
{
    auto& semaphore = *a_Semaphore;
    vk::SemaphoreSignalInfo info;
    info.semaphore = *semaphore;
    info.value     = a_Value;
    semaphore.getDevice().signalSemaphore(info);
}

void Signal(
    const std::vector<Handle>& a_Semaphores,
    const std::vector<uint64_t>& a_Values)
{
    for (auto i = 0u; i < a_Semaphores.size(); ++i) {
        Signal(a_Semaphores.at(i), a_Values.at(i));
    }
}

bool Wait(
    const Handle& a_Semaphore,
    const uint64_t& a_Value,
    const std::chrono::nanoseconds& a_TimeoutNS)
{
    auto& semaphore = *a_Semaphore;
    vk::SemaphoreWaitInfo info;
    info.semaphoreCount = 1;
    info.pValues        = &a_Value;
    info.pSemaphores    = &*semaphore;
    return semaphore.getDevice().waitSemaphores(info, a_TimeoutNS.count()) == vk::Result::eSuccess;
}

bool Wait(
    const std::vector<Handle>& a_Semaphores,
    const std::vector<uint64_t>& a_Values,
    const std::chrono::nanoseconds& a_TimeoutNS)
{
    std::vector<vk::Semaphore> vkSemaphores;
    auto& device = a_Semaphores.front()->getDevice();
    vkSemaphores.reserve(a_Semaphores.size());
    for (const auto& semaphore : a_Semaphores) {
        vkSemaphores.push_back(**semaphore);
    }
    vk::SemaphoreWaitInfo info;
    info.semaphoreCount = a_Semaphores.size();
    info.pValues        = a_Values.data();
    info.pSemaphores    = vkSemaphores.data();
    return device.waitSemaphores(info, a_TimeoutNS.count()) == vk::Result::eSuccess;
}

uint64_t GetCounterValue(const Handle& a_Semaphore)
{
    auto& semaphore = *a_Semaphore;
    return semaphore.getDevice().getSemaphoreCounterValue(*semaphore);
}

}
