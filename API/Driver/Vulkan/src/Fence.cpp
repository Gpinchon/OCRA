#include <VK/Fence.hpp>

namespace OCRA::Fence
{
static inline auto CreateFence(const VkDevice& a_Device)
{
    VkFence fence{};
    VkFenceCreateInfo info{ VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
    vkCreateFence(a_Device, &info, nullptr, &fence);
    return fence;
}

Impl::Impl(const VkDevice& a_Device)
    : device(a_Device)
    , fence(CreateFence(a_Device))
{}

}