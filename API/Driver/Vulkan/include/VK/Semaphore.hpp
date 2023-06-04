#pragma once

#include <vulkan/vulkan_raii.hpp>

namespace OCRA::Semaphore {
struct Impl : vk::raii::Semaphore {
    Impl(
        const vk::raii::Device& a_Device,
        const vk::SemaphoreCreateInfo& a_Info)
        : vk::raii::Semaphore(a_Device, a_Info)
    {
    }
};
}
