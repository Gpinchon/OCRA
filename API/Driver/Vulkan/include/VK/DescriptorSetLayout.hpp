#pragma once

#include <vulkan/vulkan.h>

namespace OCRA::Descriptor::SetLayout
{
struct Impl
{
    Impl(const VkDevice& a_Device, const VkDescriptorSetLayout& a_Layout)
        : device(a_Device)
        , layout(a_Layout)
    {}
    ~Impl() {
        vkDestroyDescriptorSetLayout(device, layout, nullptr);
    }
    operator auto& () const {
        return layout;
    }
    const VkDevice              device;
    const VkDescriptorSetLayout layout;
};
}
