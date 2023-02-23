#include <VK/Semaphore.hpp>

#include <OCRA/Semaphore.hpp>

namespace OCRA::Semaphore {
static inline auto CreateSemaphore(const VkDevice& a_Device, const Info& a_Info) {
    VkSemaphore semaphore{};
    VkSemaphoreCreateInfo info{ VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
    VkSemaphoreTypeCreateInfo semaphoreType{ VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO };
    semaphoreType.semaphoreType = a_Info.type == Type::Binary ? VK_SEMAPHORE_TYPE_BINARY : VK_SEMAPHORE_TYPE_TIMELINE;
    semaphoreType.initialValue  = a_Info.initialValue;
    info.pNext = &semaphoreType;
    vkCreateSemaphore(a_Device, &info, nullptr, &semaphore);
    return semaphore;
}
Impl::Impl(const VkDevice& a_Device, const Info& a_Info)
    : device(a_Device)
    , semaphore(CreateSemaphore(a_Device, a_Info))
{}
}
