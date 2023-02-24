#pragma once

#include <vulkan/vulkan.h>

#include <memory_resource>

namespace OCRA::Descriptor::Pool
{
struct Impl
{
    Impl(const VkDevice& a_Device, const VkDescriptorPool& a_Pool)
        : device(a_Device)
        , pool(a_Pool)
    {}
    ~Impl() {
        vkDestroyDescriptorPool(device, pool, nullptr);
    }
    operator auto& () const {
        return pool;
    }
    const VkDevice         device;
    const VkDescriptorPool pool;
    std::pmr::unsynchronized_pool_resource memoryResource;
};
}