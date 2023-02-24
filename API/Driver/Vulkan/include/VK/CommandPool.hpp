#pragma once

#include <vulkan/vulkan.h>

#include <memory_resource>

namespace OCRA::Command::Pool
{
struct Impl
{
    Impl(const VkDevice& a_Device, const VkCommandPool& a_Pool)
        : device(a_Device)
        , pool(a_Pool)
    {}
    ~Impl() {
        vkDestroyCommandPool(device, pool, nullptr);
    }
    operator auto& () const {
        return pool;
    }
    const VkDevice      device;
    const VkCommandPool pool;
    std::pmr::unsynchronized_pool_resource memoryResource;
};
}