#pragma once

#include <vulkan/vulkan.h>

namespace OCRA::Surface
{
struct Impl
{
    Impl(const VkInstance& a_Instance, const VkSurfaceKHR& a_Surface)
        : instance(a_Instance)
        , surface(a_Surface)
    {}
    ~Impl() {
        vkDestroySurfaceKHR(instance, surface, nullptr);
    }
    operator auto& () const {
        return surface;
    }
    const VkInstance   instance;
    const VkSurfaceKHR surface;

};
}