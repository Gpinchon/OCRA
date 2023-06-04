#pragma once

#include <vulkan/vulkan_raii.hpp>

namespace OCRA::Surface {
struct Impl : vk::raii::SurfaceKHR {
    Impl(
        const vk::raii::Instance& a_Instance,
        const VkSurfaceKHR& a_Surface)
        : vk::raii::SurfaceKHR(a_Instance, a_Surface)
    {
    }
};
}
