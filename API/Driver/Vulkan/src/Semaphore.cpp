#include <VK/Semaphore.hpp>
#include <VK/Device.hpp>

#include <OCRA/Structs.hpp>

#include <chrono>

namespace OCRA::Device
{
Semaphore::Handle CreateSemaphore(
    const Device::Handle& a_Device,
    const CreateSemaphoreInfo& a_Info,
    const AllocationCallback* a_Allocator)
{
    VkSemaphore semaphore{};
    VkSemaphoreCreateInfo info{ VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
    VkSemaphoreTypeCreateInfo semaphoreType{ VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO };
    semaphoreType.semaphoreType = a_Info.type == SemaphoreType::Binary ? VK_SEMAPHORE_TYPE_BINARY : VK_SEMAPHORE_TYPE_TIMELINE;
    semaphoreType.initialValue = a_Info.initialValue;
    info.pNext = &semaphoreType;
    vkCreateSemaphore(*a_Device, &info, nullptr, &semaphore);
    return std::make_shared<Semaphore::Impl>(*a_Device, semaphore);
}
}

namespace OCRA::Semaphore {
void Signal(
    const Handle& a_Semaphore,
    const uint64_t& a_Value)
{
    const auto& semaphore = *a_Semaphore;
    VkSemaphoreSignalInfo info{ VK_STRUCTURE_TYPE_SEMAPHORE_SIGNAL_INFO };
    info.semaphore = semaphore;
    info.value = a_Value;
    vkSignalSemaphore(semaphore.device, &info);
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
    const auto& semaphore = *a_Semaphore;
    VkSemaphoreWaitInfo info{ VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO };
    info.semaphoreCount = 1;
    info.pValues = &a_Value;
    info.pSemaphores = &(VkSemaphore&)semaphore;
    return vkWaitSemaphores(semaphore.device, &info, a_TimeoutNS.count()) == VK_SUCCESS;
}

bool Wait(
    const std::vector<Handle>& a_Semaphores,
    const std::vector<uint64_t>& a_Values,
    const std::chrono::nanoseconds& a_TimeoutNS)
{
    std::vector<VkSemaphore> vkSemaphores;
    const auto& device = a_Semaphores.front()->device;
    vkSemaphores.reserve(a_Semaphores.size());
    for (const auto& semaphore : a_Semaphores) {
        vkSemaphores.push_back(*semaphore);
    }
    VkSemaphoreWaitInfo info{ VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO };
    info.semaphoreCount = a_Semaphores.size();
    info.pValues = a_Values.data();
    info.pSemaphores = vkSemaphores.data();
    return vkWaitSemaphores(device, &info, a_TimeoutNS.count()) == VK_SUCCESS;
}

uint64_t GetCounterValue(const Handle& a_Semaphore)
{
    const auto& semaphore = *a_Semaphore;
    uint64_t value = 0;
    vkGetSemaphoreCounterValue(semaphore.device, semaphore, &value);
    return value;
}

}
