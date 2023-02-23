#pragma once

#include <vulkan/vulkan.hpp>

namespace OCRA::Semaphore
{
struct Info;
struct Impl {
    Impl(const VkDevice& a_Device, const Info& a_Info);
    ~Impl() {
        vkDestroySemaphore(device, semaphore, nullptr);
    }
    operator VkSemaphore() const {
        return semaphore;
    }
    const VkDevice    device;
    const VkSemaphore semaphore;
};
}