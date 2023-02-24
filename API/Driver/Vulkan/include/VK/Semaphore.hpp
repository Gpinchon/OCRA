#pragma once

#include <vulkan/vulkan.hpp>

namespace OCRA::Semaphore
{
struct Impl
{
    Impl(const VkDevice& a_Device, const VkSemaphore& a_Semaphore)
        : device(a_Device)
        , semaphore(a_Semaphore)
    {}
    ~Impl() {
        vkDestroySemaphore(device, semaphore, nullptr);
    }
    operator auto&() const {
        return semaphore;
    }
    const VkDevice    device;
    const VkSemaphore semaphore;
};
}