#pragma once

#include <vulkan/vulkan.h>

namespace OCRA::Descriptor::Set
{
struct Impl
{
    Impl(const VkDevice& a_Device, const VkDescriptorPool& a_Pool, const VkDescriptorSet& a_Set)
        : device(a_Device)
        , pool(a_Pool)
        , set(a_Set)
    {}
    ~Impl() {
        vkFreeDescriptorSets(device, pool, 1, &set);
    }
    operator auto& () const {
        return set;
    }
    const VkDevice         device;
    const VkDescriptorPool pool;
    const VkDescriptorSet  set;
};
}
