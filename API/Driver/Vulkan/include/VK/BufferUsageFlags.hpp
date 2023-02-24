#pragma once

#include <OCRA/Flags.hpp>

#include <vulkan/vulkan.h>

namespace OCRA
{
static inline auto GetVkBufferUsageFlags(const BufferUsageFlags& a_Flags) {
    VkBufferUsageFlags vkFlags = 0;
    vkFlags |= (a_Flags & BufferUsageFlagBits::TransferSrc)         == BufferUsageFlagBits::TransferSrc         ? VK_BUFFER_USAGE_TRANSFER_SRC_BIT          : 0;
    vkFlags |= (a_Flags & BufferUsageFlagBits::TransferDst)         == BufferUsageFlagBits::TransferDst         ? VK_BUFFER_USAGE_TRANSFER_DST_BIT          : 0;
    vkFlags |= (a_Flags & BufferUsageFlagBits::UniformTexelBuffer)  == BufferUsageFlagBits::UniformTexelBuffer  ? VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT  : 0;
    vkFlags |= (a_Flags & BufferUsageFlagBits::StorageTexelBuffer)  == BufferUsageFlagBits::StorageTexelBuffer  ? VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT  : 0;
    vkFlags |= (a_Flags & BufferUsageFlagBits::UniformBuffer)       == BufferUsageFlagBits::UniformBuffer       ? VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT        : 0;
    vkFlags |= (a_Flags & BufferUsageFlagBits::StorageBuffer)       == BufferUsageFlagBits::StorageBuffer       ? VK_BUFFER_USAGE_STORAGE_BUFFER_BIT        : 0;
    vkFlags |= (a_Flags & BufferUsageFlagBits::IndexBuffer)         == BufferUsageFlagBits::IndexBuffer         ? VK_BUFFER_USAGE_INDEX_BUFFER_BIT          : 0;
    vkFlags |= (a_Flags & BufferUsageFlagBits::VertexBuffer)        == BufferUsageFlagBits::VertexBuffer        ? VK_BUFFER_USAGE_VERTEX_BUFFER_BIT         : 0;
    vkFlags |= (a_Flags & BufferUsageFlagBits::IndirectBuffer)      == BufferUsageFlagBits::IndirectBuffer      ? VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT       : 0;
    vkFlags |= (a_Flags & BufferUsageFlagBits::ShaderDeviceAddress) == BufferUsageFlagBits::ShaderDeviceAddress ? VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT : 0;
    return vkFlags;
}
}