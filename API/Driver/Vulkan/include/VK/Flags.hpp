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
    vkFlags |= (a_Flags & CreateBufferFlagBits::SparseBinding)              != 0 ? VK_BUFFER_CREATE_SPARSE_BINDING_BIT                : 0;
    vkFlags |= (a_Flags & CreateBufferFlagBits::SparseResidency)            != 0 ? VK_BUFFER_CREATE_SPARSE_RESIDENCY_BIT              : 0;
    vkFlags |= (a_Flags & CreateBufferFlagBits::SparseAliased)              != 0 ? VK_BUFFER_CREATE_SPARSE_ALIASED_BIT                : 0;
    vkFlags |= (a_Flags & CreateBufferFlagBits::Protected)                  != 0 ? VK_BUFFER_CREATE_PROTECTED_BIT                     : 0;
    vkFlags |= (a_Flags & CreateBufferFlagBits::DeviceAddressCaptureReplay) != 0 ? VK_BUFFER_CREATE_DEVICE_ADDRESS_CAPTURE_REPLAY_BIT : 0;
   return vkFlags;
}
static inline auto GetVkBufferUsageFlags(const BufferUsageFlags& a_Flags) {
    VkBufferUsageFlags vkFlags = 0;
    vkFlags |= (a_Flags & BufferUsageFlagBits::TransferSrc)         != 0 ? VK_BUFFER_USAGE_TRANSFER_SRC_BIT          : 0;
    vkFlags |= (a_Flags & BufferUsageFlagBits::TransferDst)         != 0 ? VK_BUFFER_USAGE_TRANSFER_DST_BIT          : 0;
    vkFlags |= (a_Flags & BufferUsageFlagBits::UniformTexelBuffer)  != 0 ? VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT  : 0;
    vkFlags |= (a_Flags & BufferUsageFlagBits::StorageTexelBuffer)  != 0 ? VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT  : 0;
    vkFlags |= (a_Flags & BufferUsageFlagBits::UniformBuffer)       != 0 ? VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT        : 0;
    vkFlags |= (a_Flags & BufferUsageFlagBits::StorageBuffer)       != 0 ? VK_BUFFER_USAGE_STORAGE_BUFFER_BIT        : 0;
    vkFlags |= (a_Flags & BufferUsageFlagBits::IndexBuffer)         != 0 ? VK_BUFFER_USAGE_INDEX_BUFFER_BIT          : 0;
    vkFlags |= (a_Flags & BufferUsageFlagBits::VertexBuffer)        != 0 ? VK_BUFFER_USAGE_VERTEX_BUFFER_BIT         : 0;
    vkFlags |= (a_Flags & BufferUsageFlagBits::IndirectBuffer)      != 0 ? VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT       : 0;
    vkFlags |= (a_Flags & BufferUsageFlagBits::ShaderDeviceAddress) != 0 ? VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT : 0;
    return vkFlags;
}
static inline auto GetVKMemoryPropertyFlags(const MemoryPropertyFlags a_Flags) {
    VkMemoryPropertyFlags vkFlags = 0;
    vkFlags |= (a_Flags & MemoryPropertyFlagBits::DeviceLocal)     != 0 ? VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT     : 0;
    vkFlags |= (a_Flags & MemoryPropertyFlagBits::HostCached)      != 0 ? VK_MEMORY_PROPERTY_HOST_CACHED_BIT      : 0;
    vkFlags |= (a_Flags & MemoryPropertyFlagBits::HostCoherent)    != 0 ? VK_MEMORY_PROPERTY_HOST_COHERENT_BIT    : 0;
    vkFlags |= (a_Flags & MemoryPropertyFlagBits::HostVisible)     != 0 ? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT     : 0;
    vkFlags |= (a_Flags & MemoryPropertyFlagBits::LazilyAllocated) != 0 ? VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT : 0;
    vkFlags |= (a_Flags & MemoryPropertyFlagBits::Protected)       != 0 ? VK_MEMORY_PROPERTY_PROTECTED_BIT        : 0;
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
static inline auto GetVkCommandBufferUsageFlags(const CommandBufferUsageFlags& a_Flags) {
    VkCommandBufferUsageFlags vkFlags = 0;
    vkFlags |= (a_Flags & CommandBufferUsageFlagBits::OneTimeSubmit)      != 0 ? VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT      : 0;
    vkFlags |= (a_Flags & CommandBufferUsageFlagBits::RenderPassContinue) != 0 ? VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT : 0;
    vkFlags |= (a_Flags & CommandBufferUsageFlagBits::SimultaneousUse)    != 0 ? VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT     : 0;
    return vkFlags;
}
}