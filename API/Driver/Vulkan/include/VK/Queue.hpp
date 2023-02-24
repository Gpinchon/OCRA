#pragma once

#include <vulkan/vulkan.hpp>

namespace OCRA::Queue
{
struct Impl
{
    Impl(const VkQueue& a_Queue)
        : queue(a_Queue)
    {}
    operator auto&() const {
        return queue;
    }
    const VkQueue queue;
};
}