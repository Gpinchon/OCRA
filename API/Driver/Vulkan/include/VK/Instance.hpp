#pragma once

#include <vulkan/vulkan.h>

namespace OCRA::Instance
{
struct Impl
{
    Impl(const VkInstance& a_Instance)
        : instance(a_Instance)
    {}
    ~Impl() {
        vkDestroyInstance(instance, nullptr);
    }
    operator auto& () const {
        return instance;
    }
    const VkInstance instance;
};
}