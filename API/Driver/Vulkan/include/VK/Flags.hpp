/**
 * This header contains functions to convert OCRA flags into VK flags
 */
#pragma once

#include <OCRA/Flags.hpp>

#include <vulkan/vulkan.h>

namespace OCRA
{
static inline auto GetVkBufferCreateFlags(const CreateBufferFlags& a_Flags)
{
    VkBufferCreateFlags vkFlags = 0;
    vkFlags |= (a_Flags & CreateBufferFlagBits::SparseBinding)              == CreateBufferFlagBits::SparseBinding              ? VK_BUFFER_CREATE_SPARSE_BINDING_BIT                : 0;
    vkFlags |= (a_Flags & CreateBufferFlagBits::SparseResidency)            == CreateBufferFlagBits::SparseResidency            ? VK_BUFFER_CREATE_SPARSE_RESIDENCY_BIT              : 0;
    vkFlags |= (a_Flags & CreateBufferFlagBits::SparseAliased)              == CreateBufferFlagBits::SparseAliased              ? VK_BUFFER_CREATE_SPARSE_ALIASED_BIT                : 0;
    vkFlags |= (a_Flags & CreateBufferFlagBits::Protected)                  == CreateBufferFlagBits::Protected                  ? VK_BUFFER_CREATE_PROTECTED_BIT                     : 0;
    vkFlags |= (a_Flags & CreateBufferFlagBits::DeviceAddressCaptureReplay) == CreateBufferFlagBits::DeviceAddressCaptureReplay ? VK_BUFFER_CREATE_DEVICE_ADDRESS_CAPTURE_REPLAY_BIT : 0;
   return vkFlags;
}
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
static inline auto GetVkShaderStage(const ShaderStageFlags& a_Flags) {
    VkShaderStageFlags vkFlags = 0;
    vkFlags |= (a_Flags & ShaderStageFlagBits::Compute)     != 0 ? VK_SHADER_STAGE_COMPUTE_BIT                 : 0;
    vkFlags |= (a_Flags & ShaderStageFlagBits::Fragment)    != 0 ? VK_SHADER_STAGE_FRAGMENT_BIT                : 0;
    vkFlags |= (a_Flags & ShaderStageFlagBits::Geometry)    != 0 ? VK_SHADER_STAGE_GEOMETRY_BIT                : 0;
    vkFlags |= (a_Flags & ShaderStageFlagBits::TessControl) != 0 ? VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT    : 0;
    vkFlags |= (a_Flags & ShaderStageFlagBits::TessEval)    != 0 ? VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT : 0;
    vkFlags |= (a_Flags & ShaderStageFlagBits::Vertex)      != 0 ? VK_SHADER_STAGE_VERTEX_BIT                  : 0;
    return vkFlags;
}
}