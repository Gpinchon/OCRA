/**
 * This header contains functions to convert OCRA flags into VK flags
 */
#pragma once

#include <OCRA/Flags.hpp>

#include <vulkan/vulkan.hpp>

#include <stdexcept>

namespace OCRA
{
static inline auto ConvertFromVk(const vk::MemoryHeapFlags& a_Flags) {
    MemoryHeapFlags flags = 0;
    flags |= a_Flags & vk::MemoryHeapFlagBits::eDeviceLocal   ? MemoryHeapFlagBits::DeviceLocal   : MemoryHeapFlags{};
    flags |= a_Flags & vk::MemoryHeapFlagBits::eMultiInstance ? MemoryHeapFlagBits::MultiInstance : MemoryHeapFlags{};
    return flags;
}
static inline auto ConvertFromVk(const vk::MemoryPropertyFlags& a_Flags) {
    MemoryPropertyFlags vkFlags;
    vkFlags |= a_Flags & vk::MemoryPropertyFlagBits::eDeviceLocal     ? MemoryPropertyFlagBits::DeviceLocal     : MemoryPropertyFlags{};
    vkFlags |= a_Flags & vk::MemoryPropertyFlagBits::eHostCached      ? MemoryPropertyFlagBits::HostCached      : MemoryPropertyFlags{};
    vkFlags |= a_Flags & vk::MemoryPropertyFlagBits::eHostCoherent    ? MemoryPropertyFlagBits::HostCoherent    : MemoryPropertyFlags{};
    vkFlags |= a_Flags & vk::MemoryPropertyFlagBits::eHostVisible     ? MemoryPropertyFlagBits::HostVisible     : MemoryPropertyFlags{};
    vkFlags |= a_Flags & vk::MemoryPropertyFlagBits::eLazilyAllocated ? MemoryPropertyFlagBits::LazilyAllocated : MemoryPropertyFlags{};
    vkFlags |= a_Flags & vk::MemoryPropertyFlagBits::eProtected       ? MemoryPropertyFlagBits::Protected       : MemoryPropertyFlags{};
    return vkFlags;
}
static inline auto ConvertFromVk(const vk::QueueFlags& a_Flags) {
    QueueFlags ocFlags = 0;
    ocFlags |= (a_Flags & vk::QueueFlagBits::eCompute)       ? QueueFlagBits::Compute       : QueueFlags{};
    ocFlags |= (a_Flags & vk::QueueFlagBits::eGraphics)      ? QueueFlagBits::Graphics      : QueueFlags{};
    ocFlags |= (a_Flags & vk::QueueFlagBits::eProtected)     ? QueueFlagBits::Protected     : QueueFlags{};
    ocFlags |= (a_Flags & vk::QueueFlagBits::eSparseBinding) ? QueueFlagBits::SparseBinding : QueueFlags{};
    ocFlags |= (a_Flags & vk::QueueFlagBits::eTransfer)      ? QueueFlagBits::Transfer      : QueueFlags{};
    return ocFlags;
}

static inline auto ConvertToVk(const AccessFlags& a_Flags) {
    vk::AccessFlags vkFlags;
    vkFlags |= (a_Flags & AccessFlagBits::IndirectCommandRead)         != 0 ? vk::AccessFlagBits::eIndirectCommandRead         : vk::AccessFlags{};
    vkFlags |= (a_Flags & AccessFlagBits::IndexRead)                   != 0 ? vk::AccessFlagBits::eIndexRead                   : vk::AccessFlags{};
    vkFlags |= (a_Flags & AccessFlagBits::VertexAttributeRead)         != 0 ? vk::AccessFlagBits::eVertexAttributeRead         : vk::AccessFlags{};
    vkFlags |= (a_Flags & AccessFlagBits::UniformRead)                 != 0 ? vk::AccessFlagBits::eUniformRead                 : vk::AccessFlags{};
    vkFlags |= (a_Flags & AccessFlagBits::InputAttachmentRead)         != 0 ? vk::AccessFlagBits::eInputAttachmentRead         : vk::AccessFlags{};
    vkFlags |= (a_Flags & AccessFlagBits::ShaderRead)                  != 0 ? vk::AccessFlagBits::eShaderRead                  : vk::AccessFlags{};
    vkFlags |= (a_Flags & AccessFlagBits::ShaderWrite)                 != 0 ? vk::AccessFlagBits::eShaderWrite                 : vk::AccessFlags{};
    vkFlags |= (a_Flags & AccessFlagBits::ColorAttachmentRead)         != 0 ? vk::AccessFlagBits::eColorAttachmentRead         : vk::AccessFlags{};
    vkFlags |= (a_Flags & AccessFlagBits::ColorAttachmentWrite)        != 0 ? vk::AccessFlagBits::eColorAttachmentWrite        : vk::AccessFlags{};
    vkFlags |= (a_Flags & AccessFlagBits::DepthStencilAttachmentRead)  != 0 ? vk::AccessFlagBits::eDepthStencilAttachmentRead  : vk::AccessFlags{};
    vkFlags |= (a_Flags & AccessFlagBits::DepthStencilAttachmentWrite) != 0 ? vk::AccessFlagBits::eDepthStencilAttachmentWrite : vk::AccessFlags{};
    vkFlags |= (a_Flags & AccessFlagBits::TransferRead)                != 0 ? vk::AccessFlagBits::eTransferRead                : vk::AccessFlags{};
    vkFlags |= (a_Flags & AccessFlagBits::TransferWrite)               != 0 ? vk::AccessFlagBits::eTransferWrite               : vk::AccessFlags{};
    vkFlags |= (a_Flags & AccessFlagBits::HostRead)                    != 0 ? vk::AccessFlagBits::eHostRead                    : vk::AccessFlags{};
    vkFlags |= (a_Flags & AccessFlagBits::HostWrite)                   != 0 ? vk::AccessFlagBits::eHostWrite                   : vk::AccessFlags{};
    vkFlags |= (a_Flags & AccessFlagBits::MemoryRead)                  != 0 ? vk::AccessFlagBits::eMemoryRead                  : vk::AccessFlags{};
    vkFlags |= (a_Flags & AccessFlagBits::MemoryWrite)                 != 0 ? vk::AccessFlagBits::eMemoryWrite                 : vk::AccessFlags{};
    return vkFlags;
}
static inline auto ConvertToVk(const CreateBufferFlags& a_Flags)
{
    vk::BufferCreateFlags vkFlags;
    vkFlags |= (a_Flags & CreateBufferFlagBits::SparseBinding)              != 0 ? vk::BufferCreateFlagBits::eSparseBinding              : vk::BufferCreateFlags{};
    vkFlags |= (a_Flags & CreateBufferFlagBits::SparseResidency)            != 0 ? vk::BufferCreateFlagBits::eSparseResidency            : vk::BufferCreateFlags{};
    vkFlags |= (a_Flags & CreateBufferFlagBits::SparseAliased)              != 0 ? vk::BufferCreateFlagBits::eSparseAliased              : vk::BufferCreateFlags{};
    vkFlags |= (a_Flags & CreateBufferFlagBits::Protected)                  != 0 ? vk::BufferCreateFlagBits::eProtected                  : vk::BufferCreateFlags{};
    vkFlags |= (a_Flags & CreateBufferFlagBits::DeviceAddressCaptureReplay) != 0 ? vk::BufferCreateFlagBits::eDeviceAddressCaptureReplay : vk::BufferCreateFlags{};
   return vkFlags;
}
static inline auto ConvertToVk(const ColorComponentFlags& a_Flags) {
    vk::ColorComponentFlags vkFlags;
    vkFlags |= (a_Flags & ColorComponentFlagBits::R) != 0 ? vk::ColorComponentFlagBits::eR : vk::ColorComponentFlags{};
    vkFlags |= (a_Flags & ColorComponentFlagBits::G) != 0 ? vk::ColorComponentFlagBits::eG : vk::ColorComponentFlags{};
    vkFlags |= (a_Flags & ColorComponentFlagBits::B) != 0 ? vk::ColorComponentFlagBits::eB : vk::ColorComponentFlags{};
    vkFlags |= (a_Flags & ColorComponentFlagBits::A) != 0 ? vk::ColorComponentFlagBits::eA : vk::ColorComponentFlags{};
    return vkFlags;
}
static inline auto ConvertToVk(const BufferUsageFlags& a_Flags) {
    vk::BufferUsageFlags vkFlags;
    vkFlags |= (a_Flags & BufferUsageFlagBits::TransferSrc)         != 0 ? vk::BufferUsageFlagBits::eTransferSrc         : vk::BufferUsageFlags{};
    vkFlags |= (a_Flags & BufferUsageFlagBits::TransferDst)         != 0 ? vk::BufferUsageFlagBits::eTransferDst         : vk::BufferUsageFlags{};
    vkFlags |= (a_Flags & BufferUsageFlagBits::UniformTexelBuffer)  != 0 ? vk::BufferUsageFlagBits::eUniformTexelBuffer  : vk::BufferUsageFlags{};
    vkFlags |= (a_Flags & BufferUsageFlagBits::StorageTexelBuffer)  != 0 ? vk::BufferUsageFlagBits::eStorageTexelBuffer  : vk::BufferUsageFlags{};
    vkFlags |= (a_Flags & BufferUsageFlagBits::UniformBuffer)       != 0 ? vk::BufferUsageFlagBits::eUniformBuffer       : vk::BufferUsageFlags{};
    vkFlags |= (a_Flags & BufferUsageFlagBits::StorageBuffer)       != 0 ? vk::BufferUsageFlagBits::eStorageBuffer       : vk::BufferUsageFlags{};
    vkFlags |= (a_Flags & BufferUsageFlagBits::IndexBuffer)         != 0 ? vk::BufferUsageFlagBits::eIndexBuffer         : vk::BufferUsageFlags{};
    vkFlags |= (a_Flags & BufferUsageFlagBits::VertexBuffer)        != 0 ? vk::BufferUsageFlagBits::eVertexBuffer        : vk::BufferUsageFlags{};
    vkFlags |= (a_Flags & BufferUsageFlagBits::IndirectBuffer)      != 0 ? vk::BufferUsageFlagBits::eIndirectBuffer      : vk::BufferUsageFlags{};
    vkFlags |= (a_Flags & BufferUsageFlagBits::ShaderDeviceAddress) != 0 ? vk::BufferUsageFlagBits::eShaderDeviceAddress : vk::BufferUsageFlags{};
    return vkFlags;
}
static inline auto ConvertToVk(const CommandBufferUsageFlags& a_Flags) {
    vk::CommandBufferUsageFlags vkFlags;
    vkFlags |= (a_Flags & CommandBufferUsageFlagBits::OneTimeSubmit)      != 0 ? vk::CommandBufferUsageFlagBits::eOneTimeSubmit      : vk::CommandBufferUsageFlags{};
    vkFlags |= (a_Flags & CommandBufferUsageFlagBits::RenderPassContinue) != 0 ? vk::CommandBufferUsageFlagBits::eRenderPassContinue : vk::CommandBufferUsageFlags{};
    vkFlags |= (a_Flags & CommandBufferUsageFlagBits::SimultaneousUse)    != 0 ? vk::CommandBufferUsageFlagBits::eSimultaneousUse    : vk::CommandBufferUsageFlags{};
    return vkFlags;
}
static inline auto ConvertToVk(const CreateCommandPoolFlags& a_Flags) {
    vk::CommandPoolCreateFlags vkFlags;
    vkFlags |= (a_Flags & CreateCommandPoolFlagBits::Protected) == CreateCommandPoolFlagBits::Protected ? vk::CommandPoolCreateFlagBits::eProtected          : vk::CommandPoolCreateFlags{};
    vkFlags |= (a_Flags & CreateCommandPoolFlagBits::Transient) == CreateCommandPoolFlagBits::Transient ? vk::CommandPoolCreateFlagBits::eTransient          : vk::CommandPoolCreateFlags{};
    vkFlags |= (a_Flags & CreateCommandPoolFlagBits::Reset)     == CreateCommandPoolFlagBits::Reset     ? vk::CommandPoolCreateFlagBits::eResetCommandBuffer : vk::CommandPoolCreateFlags{};
    return vkFlags;
}
static inline auto ConvertToVk(const DependencyFlags& a_Flags) {
    vk::DependencyFlags vkFlags;
    vkFlags |= (a_Flags & DependencyFlagBits::ByRegion)    != 0 ? vk::DependencyFlagBits::eByRegion    : vk::DependencyFlags{};
    vkFlags |= (a_Flags & DependencyFlagBits::DeviceGroup) != 0 ? vk::DependencyFlagBits::eDeviceGroup : vk::DependencyFlags{};
    vkFlags |= (a_Flags & DependencyFlagBits::ViewLocal)   != 0 ? vk::DependencyFlagBits::eViewLocal   : vk::DependencyFlags{};
    return vkFlags;
}

static inline auto ConvertToVk(const ImageAspectFlags& a_Flags) {
    vk::ImageAspectFlags vkFlags;
    vkFlags |= (a_Flags & ImageAspectFlagBits::Color)    != 0 ? vk::ImageAspectFlagBits::eColor    : vk::ImageAspectFlags{};
    vkFlags |= (a_Flags & ImageAspectFlagBits::Depth)    != 0 ? vk::ImageAspectFlagBits::eDepth    : vk::ImageAspectFlags{};
    vkFlags |= (a_Flags & ImageAspectFlagBits::Stencil)  != 0 ? vk::ImageAspectFlagBits::eStencil  : vk::ImageAspectFlags{};
    vkFlags |= (a_Flags & ImageAspectFlagBits::Metadata) != 0 ? vk::ImageAspectFlagBits::eMetadata : vk::ImageAspectFlags{};
    vkFlags |= (a_Flags & ImageAspectFlagBits::Plane0)   != 0 ? vk::ImageAspectFlagBits::ePlane0   : vk::ImageAspectFlags{};
    vkFlags |= (a_Flags & ImageAspectFlagBits::Plane1)   != 0 ? vk::ImageAspectFlagBits::ePlane1   : vk::ImageAspectFlags{};
    vkFlags |= (a_Flags & ImageAspectFlagBits::Plane2)   != 0 ? vk::ImageAspectFlagBits::ePlane2   : vk::ImageAspectFlags{};
    return vkFlags;
}
static inline auto ConvertToVk(const ImageUsageFlags& a_Flags) {
    vk::ImageUsageFlags flags;
    flags |= (a_Flags & ImageUsageFlagBits::TransferSrc)            != 0 ? vk::ImageUsageFlagBits::eTransferSrc            : vk::ImageUsageFlags{};
    flags |= (a_Flags & ImageUsageFlagBits::TransferDst)            != 0 ? vk::ImageUsageFlagBits::eTransferDst            : vk::ImageUsageFlags{};
    flags |= (a_Flags & ImageUsageFlagBits::Sampled)                != 0 ? vk::ImageUsageFlagBits::eSampled                : vk::ImageUsageFlags{};
    flags |= (a_Flags & ImageUsageFlagBits::Storage)                != 0 ? vk::ImageUsageFlagBits::eStorage                : vk::ImageUsageFlags{};
    flags |= (a_Flags & ImageUsageFlagBits::ColorAttachment)        != 0 ? vk::ImageUsageFlagBits::eColorAttachment        : vk::ImageUsageFlags{};
    flags |= (a_Flags & ImageUsageFlagBits::DepthStencilAttachment) != 0 ? vk::ImageUsageFlagBits::eDepthStencilAttachment : vk::ImageUsageFlags{};
    flags |= (a_Flags & ImageUsageFlagBits::TransientAttachment)    != 0 ? vk::ImageUsageFlagBits::eTransientAttachment    : vk::ImageUsageFlags{};
    flags |= (a_Flags & ImageUsageFlagBits::InputAttachment)        != 0 ? vk::ImageUsageFlagBits::eInputAttachment        : vk::ImageUsageFlags{};
    return flags;
}
static inline auto ConvertToVk(const MemoryPropertyFlags& a_Flags) {
    vk::MemoryPropertyFlags vkFlags;
    vkFlags |= (a_Flags & MemoryPropertyFlagBits::DeviceLocal)     != 0 ? vk::MemoryPropertyFlagBits::eDeviceLocal      : vk::MemoryPropertyFlags{};
    vkFlags |= (a_Flags & MemoryPropertyFlagBits::HostCached)      != 0 ? vk::MemoryPropertyFlagBits::eHostCached       : vk::MemoryPropertyFlags{};
    vkFlags |= (a_Flags & MemoryPropertyFlagBits::HostCoherent)    != 0 ? vk::MemoryPropertyFlagBits::eHostCoherent     : vk::MemoryPropertyFlags{};
    vkFlags |= (a_Flags & MemoryPropertyFlagBits::HostVisible)     != 0 ? vk::MemoryPropertyFlagBits::eHostVisible      : vk::MemoryPropertyFlags{};
    vkFlags |= (a_Flags & MemoryPropertyFlagBits::LazilyAllocated) != 0 ? vk::MemoryPropertyFlagBits::eLazilyAllocated  : vk::MemoryPropertyFlags{};
    vkFlags |= (a_Flags & MemoryPropertyFlagBits::Protected)       != 0 ? vk::MemoryPropertyFlagBits::eProtected        : vk::MemoryPropertyFlags{};
    return vkFlags;
}
static inline auto ConvertToVk(const PipelineStageFlags& a_Flags) {
    vk::PipelineStageFlags vkFlags;
    vkFlags |= (a_Flags & PipelineStageFlagBits::BottomOfPipe)                 != 0 ? vk::PipelineStageFlagBits::eBottomOfPipe                 : vk::PipelineStageFlags{};
    vkFlags |= (a_Flags & PipelineStageFlagBits::ColorAttachmentOutput)        != 0 ? vk::PipelineStageFlagBits::eColorAttachmentOutput        : vk::PipelineStageFlags{};
    vkFlags |= (a_Flags & PipelineStageFlagBits::ComputeShader)                != 0 ? vk::PipelineStageFlagBits::eComputeShader                : vk::PipelineStageFlags{};
    vkFlags |= (a_Flags & PipelineStageFlagBits::DrawIndirect)                 != 0 ? vk::PipelineStageFlagBits::eDrawIndirect                 : vk::PipelineStageFlags{};
    vkFlags |= (a_Flags & PipelineStageFlagBits::EarlyFragmentTests)           != 0 ? vk::PipelineStageFlagBits::eEarlyFragmentTests           : vk::PipelineStageFlags{};
    vkFlags |= (a_Flags & PipelineStageFlagBits::FragmentShader)               != 0 ? vk::PipelineStageFlagBits::eFragmentShader               : vk::PipelineStageFlags{};
    vkFlags |= (a_Flags & PipelineStageFlagBits::GeometryShader)               != 0 ? vk::PipelineStageFlagBits::eGeometryShader               : vk::PipelineStageFlags{};
    vkFlags |= (a_Flags & PipelineStageFlagBits::Host)                         != 0 ? vk::PipelineStageFlagBits::eHost                         : vk::PipelineStageFlags{};
    vkFlags |= (a_Flags & PipelineStageFlagBits::LateFragmentTests)            != 0 ? vk::PipelineStageFlagBits::eLateFragmentTests            : vk::PipelineStageFlags{};
    vkFlags |= (a_Flags & PipelineStageFlagBits::TessellationControlShader)    != 0 ? vk::PipelineStageFlagBits::eTessellationControlShader    : vk::PipelineStageFlags{};
    vkFlags |= (a_Flags & PipelineStageFlagBits::TessellationEvaluationShader) != 0 ? vk::PipelineStageFlagBits::eTessellationEvaluationShader : vk::PipelineStageFlags{};
    vkFlags |= (a_Flags & PipelineStageFlagBits::TopOfPipe)                    != 0 ? vk::PipelineStageFlagBits::eTopOfPipe                    : vk::PipelineStageFlags{};
    vkFlags |= (a_Flags & PipelineStageFlagBits::Transfer)                     != 0 ? vk::PipelineStageFlagBits::eTransfer                     : vk::PipelineStageFlags{};
    vkFlags |= (a_Flags & PipelineStageFlagBits::VertexInput)                  != 0 ? vk::PipelineStageFlagBits::eVertexInput                  : vk::PipelineStageFlags{};
    vkFlags |= (a_Flags & PipelineStageFlagBits::VertexShader)                 != 0 ? vk::PipelineStageFlagBits::eVertexShader                 : vk::PipelineStageFlags{};
    return vkFlags;
}
static inline auto ConvertToVk(const QueueFlags& a_Flags) {
    vk::QueueFlags vkFlags;
    vkFlags |= (a_Flags & QueueFlagBits::Compute)       != 0 ? vk::QueueFlagBits::eCompute        : vk::QueueFlags{};
    vkFlags |= (a_Flags & QueueFlagBits::Graphics)      != 0 ? vk::QueueFlagBits::eGraphics       : vk::QueueFlags{};
    vkFlags |= (a_Flags & QueueFlagBits::Protected)     != 0 ? vk::QueueFlagBits::eProtected      : vk::QueueFlags{};
    vkFlags |= (a_Flags & QueueFlagBits::SparseBinding) != 0 ? vk::QueueFlagBits::eSparseBinding  : vk::QueueFlags{};
    vkFlags |= (a_Flags & QueueFlagBits::Transfer)      != 0 ? vk::QueueFlagBits::eTransfer       : vk::QueueFlags{};
    return vkFlags;
}
static inline auto ConvertToVk(const ShaderStageFlags& a_Flags) {
    vk::ShaderStageFlags vkFlags;
    vkFlags |= (a_Flags & ShaderStageFlagBits::Compute)     != 0 ? vk::ShaderStageFlagBits::eCompute                : vk::ShaderStageFlags{};
    vkFlags |= (a_Flags & ShaderStageFlagBits::Fragment)    != 0 ? vk::ShaderStageFlagBits::eFragment               : vk::ShaderStageFlags{};
    vkFlags |= (a_Flags & ShaderStageFlagBits::Geometry)    != 0 ? vk::ShaderStageFlagBits::eGeometry               : vk::ShaderStageFlags{};
    vkFlags |= (a_Flags & ShaderStageFlagBits::TessControl) != 0 ? vk::ShaderStageFlagBits::eTessellationControl    : vk::ShaderStageFlags{};
    vkFlags |= (a_Flags & ShaderStageFlagBits::TessEval)    != 0 ? vk::ShaderStageFlagBits::eTessellationEvaluation : vk::ShaderStageFlags{};
    vkFlags |= (a_Flags & ShaderStageFlagBits::Vertex)      != 0 ? vk::ShaderStageFlagBits::eVertex                 : vk::ShaderStageFlags{};
    return vkFlags;
}

static inline vk::AccessFlags GetImageTransitionAccessMask(const vk::ImageLayout& a_Layout)
{
    switch (a_Layout)
    {
    case vk::ImageLayout::eUndefined:
    case vk::ImageLayout::ePresentSrcKHR:
        return vk::AccessFlagBits::eNone;
    case vk::ImageLayout::eGeneral:
        return
            vk::AccessFlagBits::eColorAttachmentWrite |
            vk::AccessFlagBits::eDepthStencilAttachmentWrite |
            vk::AccessFlagBits::eTransferWrite | vk::AccessFlagBits::eTransferRead |
            vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eHostWrite |
            vk::AccessFlagBits::eHostRead | vk::AccessFlagBits::eInputAttachmentRead |
            vk::AccessFlagBits::eColorAttachmentRead |
            vk::AccessFlagBits::eDepthStencilAttachmentRead |
            vk::AccessFlagBits::eMemoryRead | vk::AccessFlagBits::eMemoryWrite;
    case vk::ImageLayout::eColorAttachmentOptimal:
        return
            vk::AccessFlagBits::eColorAttachmentRead |
            vk::AccessFlagBits::eColorAttachmentWrite;

    case vk::ImageLayout::eStencilAttachmentOptimal:
    case vk::ImageLayout::eDepthAttachmentOptimal:
    case vk::ImageLayout::eDepthStencilAttachmentOptimal:
        return
            vk::AccessFlagBits::eDepthStencilAttachmentRead |
            vk::AccessFlagBits::eDepthStencilAttachmentWrite;
    case vk::ImageLayout::eDepthAttachmentStencilReadOnlyOptimal:
    case vk::ImageLayout::eDepthReadOnlyOptimal:
    case vk::ImageLayout::eDepthReadOnlyStencilAttachmentOptimal:
    case vk::ImageLayout::eDepthStencilReadOnlyOptimal:
    case vk::ImageLayout::eStencilReadOnlyOptimal:
        return vk::AccessFlagBits::eDepthStencilAttachmentRead;

    case vk::ImageLayout::eShaderReadOnlyOptimal:
        return
            vk::AccessFlagBits::eShaderRead |
            vk::AccessFlagBits::eInputAttachmentRead;

    case vk::ImageLayout::eTransferSrcOptimal:
        return vk::AccessFlagBits::eTransferRead;
    case vk::ImageLayout::eTransferDstOptimal:
        return vk::AccessFlagBits::eTransferWrite;
    case vk::ImageLayout::ePreinitialized:
        return
            vk::AccessFlagBits::eHostWrite |
            vk::AccessFlagBits::eTransferWrite;

    case vk::ImageLayout::eReadOnlyOptimal:
        break;
    case vk::ImageLayout::eAttachmentOptimal:
        break;
    case vk::ImageLayout::eVideoDecodeDstKHR:
        break;
    case vk::ImageLayout::eVideoDecodeSrcKHR:
        break;
    case vk::ImageLayout::eVideoDecodeDpbKHR:
        break;
    case vk::ImageLayout::eSharedPresentKHR:
        return vk::AccessFlagBits::eMemoryRead;
    case vk::ImageLayout::eFragmentDensityMapOptimalEXT:
        break;
    case vk::ImageLayout::eFragmentShadingRateAttachmentOptimalKHR:
        break;
    case vk::ImageLayout::eAttachmentFeedbackLoopOptimalEXT:
        break;
    }
    throw std::runtime_error("Error: unsupported layout transition!");
    return vk::AccessFlagBits(-1);
}

static inline vk::PipelineStageFlags GetImageTransitionStage(const vk::ImageLayout& a_Stage)
{
    switch (a_Stage)
    {
    case vk::ImageLayout::eUndefined:
        return vk::PipelineStageFlagBits::eTopOfPipe;
    case vk::ImageLayout::eGeneral:
        return vk::PipelineStageFlagBits::eAllCommands;
    case vk::ImageLayout::eColorAttachmentOptimal:
        return vk::PipelineStageFlagBits::eColorAttachmentOutput;
    case vk::ImageLayout::eDepthStencilAttachmentOptimal:
        return vk::PipelineStageFlagBits::eFragmentShader;
    case vk::ImageLayout::eDepthStencilReadOnlyOptimal:
        break;
    case vk::ImageLayout::eShaderReadOnlyOptimal:
        return
            vk::PipelineStageFlagBits::eFragmentShader |
            vk::PipelineStageFlagBits::eVertexShader |
            vk::PipelineStageFlagBits::eTessellationControlShader |
            vk::PipelineStageFlagBits::eTessellationEvaluationShader;
    case vk::ImageLayout::eTransferSrcOptimal:
    case vk::ImageLayout::eTransferDstOptimal:
        return vk::PipelineStageFlagBits::eTransfer;
    case vk::ImageLayout::ePreinitialized:
        return vk::PipelineStageFlagBits::eHost;
    case vk::ImageLayout::eDepthReadOnlyStencilAttachmentOptimal:
        break;
    case vk::ImageLayout::eDepthAttachmentStencilReadOnlyOptimal:
        break;
    case vk::ImageLayout::eDepthAttachmentOptimal:
        break;
    case vk::ImageLayout::eDepthReadOnlyOptimal:
        break;
    case vk::ImageLayout::eStencilAttachmentOptimal:
        break;
    case vk::ImageLayout::eStencilReadOnlyOptimal:
        break;
    case vk::ImageLayout::eReadOnlyOptimal:
        break;
    case vk::ImageLayout::eAttachmentOptimal:
        return vk::PipelineStageFlagBits::eColorAttachmentOutput;
    case vk::ImageLayout::ePresentSrcKHR:
        return vk::PipelineStageFlagBits::eBottomOfPipe;
    case vk::ImageLayout::eVideoDecodeDstKHR:
        break;
    case vk::ImageLayout::eVideoDecodeSrcKHR:
        break;
    case vk::ImageLayout::eVideoDecodeDpbKHR:
        break;
    case vk::ImageLayout::eSharedPresentKHR:
        return vk::PipelineStageFlagBits::eBottomOfPipe;
    case vk::ImageLayout::eFragmentDensityMapOptimalEXT:
        break;
    case vk::ImageLayout::eFragmentShadingRateAttachmentOptimalKHR:
        break;
    case vk::ImageLayout::eAttachmentFeedbackLoopOptimalEXT:
        break;
    }
    throw std::runtime_error("Error: unsupported layout transition!");
    return vk::PipelineStageFlagBits(-1);
}
}