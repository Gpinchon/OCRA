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
static inline auto GetVkPipelineStageFlags(const PipelineStageFlags& a_Flags) {
    VkPipelineStageFlags vkFlags = 0;
    vkFlags |= (a_Flags & PipelineStageFlagBits::BottomOfPipe)                 != 0 ? VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT                 : 0;
    vkFlags |= (a_Flags & PipelineStageFlagBits::ColorAttachmentOutput)        != 0 ? VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT        : 0;
    vkFlags |= (a_Flags & PipelineStageFlagBits::ComputeShader)                != 0 ? VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT                 : 0;
    vkFlags |= (a_Flags & PipelineStageFlagBits::DrawIndirect)                 != 0 ? VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT                  : 0;
    vkFlags |= (a_Flags & PipelineStageFlagBits::EarlyFragmentTests)           != 0 ? VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT           : 0;
    vkFlags |= (a_Flags & PipelineStageFlagBits::FragmentShader)               != 0 ? VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT                : 0;
    vkFlags |= (a_Flags & PipelineStageFlagBits::GeometryShader)               != 0 ? VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT                : 0;
    vkFlags |= (a_Flags & PipelineStageFlagBits::Host)                         != 0 ? VK_PIPELINE_STAGE_HOST_BIT                           : 0;
    vkFlags |= (a_Flags & PipelineStageFlagBits::LateFragmentTests)            != 0 ? VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT            : 0;
    vkFlags |= (a_Flags & PipelineStageFlagBits::TessellationControlShader)    != 0 ? VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT    : 0;
    vkFlags |= (a_Flags & PipelineStageFlagBits::TessellationEvaluationShader) != 0 ? VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT : 0;
    vkFlags |= (a_Flags & PipelineStageFlagBits::TopOfPipe)                    != 0 ? VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT                    : 0;
    vkFlags |= (a_Flags & PipelineStageFlagBits::Transfer)                     != 0 ? VK_PIPELINE_STAGE_TRANSFER_BIT                       : 0;
    vkFlags |= (a_Flags & PipelineStageFlagBits::VertexInput)                  != 0 ? VK_PIPELINE_STAGE_VERTEX_INPUT_BIT                   : 0;
    vkFlags |= (a_Flags & PipelineStageFlagBits::VertexShader)                 != 0 ? VK_PIPELINE_STAGE_VERTEX_SHADER_BIT                  : 0;
    return vkFlags;
}
static inline auto GetVkDependencyFlags(const DependencyFlags& a_Flags) {
    VkDependencyFlags vkFlags = 0;
    vkFlags |= (a_Flags & DependencyFlagBits::ByRegion)    != 0 ? VK_DEPENDENCY_BY_REGION_BIT    : 0;
    vkFlags |= (a_Flags & DependencyFlagBits::DeviceGroup) != 0 ? VK_DEPENDENCY_DEVICE_GROUP_BIT : 0;
    vkFlags |= (a_Flags & DependencyFlagBits::ViewLocal)   != 0 ? VK_DEPENDENCY_VIEW_LOCAL_BIT   : 0;
    return vkFlags;
}
static inline auto GetVkAccessMaskFlags(const AccessFlags& a_Flags) {
    VkAccessFlags vkFlags = 0;
    vkFlags |= (a_Flags & AccessFlagBits::IndirectCommandRead)         != 0 ? VK_ACCESS_INDIRECT_COMMAND_READ_BIT          : 0;
    vkFlags |= (a_Flags & AccessFlagBits::IndexRead)                   != 0 ? VK_ACCESS_INDEX_READ_BIT                     : 0;
    vkFlags |= (a_Flags & AccessFlagBits::VertexAttributeRead)         != 0 ? VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT          : 0;
    vkFlags |= (a_Flags & AccessFlagBits::UniformRead)                 != 0 ? VK_ACCESS_UNIFORM_READ_BIT                   : 0;
    vkFlags |= (a_Flags & AccessFlagBits::InputAttachmentRead)         != 0 ? VK_ACCESS_INPUT_ATTACHMENT_READ_BIT          : 0;
    vkFlags |= (a_Flags & AccessFlagBits::ShaderRead)                  != 0 ? VK_ACCESS_SHADER_READ_BIT                    : 0;
    vkFlags |= (a_Flags & AccessFlagBits::ShaderWrite)                 != 0 ? VK_ACCESS_SHADER_WRITE_BIT                   : 0;
    vkFlags |= (a_Flags & AccessFlagBits::ColorAttachmentRead)         != 0 ? VK_ACCESS_COLOR_ATTACHMENT_READ_BIT          : 0;
    vkFlags |= (a_Flags & AccessFlagBits::ColorAttachmentWrite)        != 0 ? VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT         : 0;
    vkFlags |= (a_Flags & AccessFlagBits::DepthStencilAttachmentRead)  != 0 ? VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT  : 0;
    vkFlags |= (a_Flags & AccessFlagBits::DepthStencilAttachmentWrite) != 0 ? VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT : 0;
    vkFlags |= (a_Flags & AccessFlagBits::TransferRead)                != 0 ? VK_ACCESS_TRANSFER_READ_BIT                  : 0;
    vkFlags |= (a_Flags & AccessFlagBits::TransferWrite)               != 0 ? VK_ACCESS_TRANSFER_WRITE_BIT                 : 0;
    vkFlags |= (a_Flags & AccessFlagBits::HostRead)                    != 0 ? VK_ACCESS_HOST_READ_BIT                      : 0;
    vkFlags |= (a_Flags & AccessFlagBits::HostWrite)                   != 0 ? VK_ACCESS_HOST_WRITE_BIT                     : 0;
    vkFlags |= (a_Flags & AccessFlagBits::MemoryRead)                  != 0 ? VK_ACCESS_MEMORY_READ_BIT                    : 0;
    vkFlags |= (a_Flags & AccessFlagBits::MemoryWrite)                 != 0 ? VK_ACCESS_MEMORY_WRITE_BIT                   : 0;
    return vkFlags;
}
static inline auto GetVkImageUsage(const ImageUsageFlags& a_Flags) {
    VkImageUsageFlags vkFlags = 0;
    vkFlags |= (a_Flags & ImageUsageFlagBits::TransferSrc)            != 0 ? VK_IMAGE_USAGE_TRANSFER_SRC_BIT             : 0;
    vkFlags |= (a_Flags & ImageUsageFlagBits::TransferDst)            != 0 ? VK_IMAGE_USAGE_TRANSFER_DST_BIT             : 0;
    vkFlags |= (a_Flags & ImageUsageFlagBits::Sampled)                != 0 ? VK_IMAGE_USAGE_SAMPLED_BIT                  : 0;
    vkFlags |= (a_Flags & ImageUsageFlagBits::Storage)                != 0 ? VK_IMAGE_USAGE_STORAGE_BIT                  : 0;
    vkFlags |= (a_Flags & ImageUsageFlagBits::ColorAttachment)        != 0 ? VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT         : 0;
    vkFlags |= (a_Flags & ImageUsageFlagBits::DepthStencilAttachment) != 0 ? VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT : 0;
    vkFlags |= (a_Flags & ImageUsageFlagBits::TransientAttachment)    != 0 ? VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT     : 0;
    vkFlags |= (a_Flags & ImageUsageFlagBits::InputAttachment)        != 0 ? VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT         : 0;
    return vkFlags;
}
}