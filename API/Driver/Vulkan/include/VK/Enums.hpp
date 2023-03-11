/**
 * @brief this header contains functions to convert OCRA enums to VK enums
 */
#pragma once

#include <OCRA/Enums.hpp>

#include <vulkan/vulkan.hpp>

namespace OCRA
{
static inline auto ConvertFromVk(const vk::ColorSpaceKHR& a_ColorSpace) {
    switch (a_ColorSpace)
    {
    case vk::ColorSpaceKHR::eExtendedSrgbLinearEXT:
        return ColorSpace::Linear;
    case vk::ColorSpaceKHR::eSrgbNonlinear:
        return ColorSpace::sRGB;
    case vk::ColorSpaceKHR::eHdr10St2084EXT:
        return ColorSpace::HDR10;
    }
    return ColorSpace::MaxValue;
}
static inline auto ConvertFromVk(const vk::Format& a_Format) {
    switch (a_Format)
    {
    case vk::Format::eR8Unorm:
        return Format::Uint8_Normalized_R;
    case vk::Format::eR8G8Unorm:
        return Format::Uint8_Normalized_RG;
    case vk::Format::eR8G8B8Unorm:
        return Format::Uint8_Normalized_RGB;
    case vk::Format::eR8G8B8A8Unorm:
        return Format::Uint8_Normalized_RGBA;
    case vk::Format::eB8G8R8Unorm:
        return Format::Uint8_Normalized_BGR;
    case vk::Format::eB8G8R8A8Unorm:
        return Format::Uint8_Normalized_BGRA;
    case vk::Format::eB8G8R8Srgb:
        return Format::Uint8_SBGR;
    case vk::Format::eB8G8R8A8Srgb:
        return Format::Uint8_SBGRA;
    case vk::Format::eR8Snorm:
        return Format::Int8_Normalized_R;
    case vk::Format::eR8G8Snorm:
        return Format::Int8_Normalized_RG;
    case vk::Format::eR8G8B8Snorm:
        return Format::Int8_Normalized_RGB;
    case vk::Format::eR8G8B8A8Snorm:
        return Format::Int8_Normalized_RGBA;
    case vk::Format::eR8Uint:
        return Format::Uint8_R;
    case vk::Format::eR8G8Uint:
        return Format::Uint8_RG;
    case vk::Format::eR8G8B8Uint:
        return Format::Uint8_RGB;
    case vk::Format::eR8G8B8A8Uint:
        return Format::Uint8_RGBA;
    case vk::Format::eR8Sint:
        return Format::Int8_R;
    case vk::Format::eR8G8Sint:
        return Format::Int8_RG;
    case vk::Format::eR8G8B8Sint:
        return Format::Int8_RGB;
    case vk::Format::eR8G8B8A8Sint:
        return Format::Int8_RGBA;
    case vk::Format::eR16Unorm:
        return Format::Uint16_Normalized_R;
    case vk::Format::eR16G16Unorm:
        return Format::Uint16_Normalized_RG;
    case vk::Format::eR16G16B16Unorm:
        return Format::Uint16_Normalized_RGB;
    case vk::Format::eR16G16B16A16Unorm:
        return Format::Uint16_Normalized_RGBA;
    case vk::Format::eR16Snorm:
        return Format::Int16_Normalized_R;
    case vk::Format::eR16G16Snorm:
        return Format::Int16_Normalized_RG;
    case vk::Format::eR16G16B16Snorm:
        return Format::Int16_Normalized_RGB;
    case vk::Format::eR16G16B16A16Snorm:
        return Format::Int16_Normalized_RGBA;
    case vk::Format::eR16Uint:
        return Format::Uint16_R;
    case vk::Format::eR16G16Uint:
        return Format::Uint16_RG;
    case vk::Format::eR16G16B16Uint:
        return Format::Uint16_RGB;
    case vk::Format::eR16G16B16A16Uint:
        return Format::Uint16_RGBA;
    case vk::Format::eR16Sint:
        return Format::Int16_R;
    case vk::Format::eR16G16Sint:
        return Format::Int16_RG;
    case vk::Format::eR16G16B16Sint:
        return Format::Int16_RGB;
    case vk::Format::eR16G16B16A16Sint:
        return Format::Int16_RGBA;
    case vk::Format::eR32Uint:
        return Format::Uint32_R;
    case vk::Format::eR32G32Uint:
        return Format::Uint32_RG;
    case vk::Format::eR32G32B32Uint:
        return Format::Uint32_RGB;
    case vk::Format::eR32G32B32A32Uint:
        return Format::Uint32_RGBA;
    case vk::Format::eR32Sint:
        return Format::Int32_R;
    case vk::Format::eR32G32Sint:
        return Format::Int32_RG;
    case vk::Format::eR32G32B32Sint:
        return Format::Int32_RGB;
    case vk::Format::eR32G32B32A32Sint:
        return Format::Int32_RGBA;
    case vk::Format::eR16Sfloat:
        return Format::Float16_R;
    case vk::Format::eR16G16Sfloat:
        return Format::Float16_RG;
    case vk::Format::eR16G16B16Sfloat:
        return Format::Float16_RGB;
    case vk::Format::eR16G16B16A16Sfloat:
        return Format::Float16_RGBA;
    case vk::Format::eR32Sfloat:
        return Format::Float32_R;
    case vk::Format::eR32G32Sfloat:
        return Format::Float32_RG;
    case vk::Format::eR32G32B32Sfloat:
        return Format::Float32_RGB;
    case vk::Format::eR32G32B32A32Sfloat:
        return Format::Float32_RGBA;
    case vk::Format::eD16Unorm:
        return Format::Uint16_Normalized_Depth;
    case vk::Format::eD32Sfloat:
        return Format::Float32_Depth;
    case vk::Format::eD24UnormS8Uint:
        return Format::Uint24_Normalized_Depth_Uint8_Stencil;
    case vk::Format::eD32SfloatS8Uint:
        return Format::Float32_Normalized_Depth_Uint8_Stencil;
    case vk::Format::eS8Uint:
        return Format::Uint8_Stencil;
    case vk::Format::eBc3UnormBlock:
        return Format::S3TC_DXT5_RGBA;
    case vk::Format::eBc3SrgbBlock:
        return Format::S3TC_DXT5_SRGBA;
    case vk::Format::eA2B10G10R10UnormPack32:
        return Format::Normalized_BGR10_A2;
    case vk::Format::eA2B10G10R10UintPack32:
        return Format::BGR10_A2;
    }
    return Format::MaxValue;
}
static inline auto ConvertFromVk(const vk::PhysicalDeviceType& a_Type)
{
    switch (a_Type)
    {
    case vk::PhysicalDeviceType::eOther:
        return PhysicalDeviceType::Other;
    case vk::PhysicalDeviceType::eIntegratedGpu:
        return PhysicalDeviceType::IntegratedGpu;
    case vk::PhysicalDeviceType::eDiscreteGpu:
        return PhysicalDeviceType::DiscreteGpu;
    case vk::PhysicalDeviceType::eVirtualGpu:
        return PhysicalDeviceType::VirtualGpu;
    case vk::PhysicalDeviceType::eCpu:
        return PhysicalDeviceType::Cpu;
    }
    return PhysicalDeviceType::MaxValue;
}

static inline auto ConvertToVk(const BlendFactor& a_BlendFactor) {
    switch (a_BlendFactor)
    {
    case BlendFactor::Zero:
        return vk::BlendFactor::eZero;
    case BlendFactor::One:
        return vk::BlendFactor::eOne;
    case BlendFactor::SrcColor:
        return vk::BlendFactor::eSrcColor;
    case BlendFactor::OneMinusSrcColor:
        return vk::BlendFactor::eOneMinusSrcColor;
    case BlendFactor::DstColor:
        return vk::BlendFactor::eDstColor;
    case BlendFactor::OneMinusDstColor:
        return vk::BlendFactor::eOneMinusDstColor;
    case BlendFactor::SrcAlpha:
        return vk::BlendFactor::eSrcAlpha;
    case BlendFactor::OneMinusSrcAlpha:
        return vk::BlendFactor::eOneMinusSrcAlpha;
    case BlendFactor::DstAlpha:
        return vk::BlendFactor::eDstAlpha;
    case BlendFactor::OneMinusDstAlpha:
        return vk::BlendFactor::eOneMinusDstAlpha;
    case BlendFactor::ConstantColor:
        return vk::BlendFactor::eConstantColor;
    case BlendFactor::OneMinusConstantColor:
        return vk::BlendFactor::eOneMinusConstantColor;
    case BlendFactor::ConstantAlpha:
        return vk::BlendFactor::eConstantAlpha;
    case BlendFactor::OneMinusConstantAlpha:
        return vk::BlendFactor::eOneMinusConstantAlpha;
    case BlendFactor::SrcAlphaSaturate:
        return vk::BlendFactor::eSrcAlphaSaturate;
    case BlendFactor::Src1Color:
        return vk::BlendFactor::eSrc1Color;
    case BlendFactor::OneMinusSrc1Color:
        return vk::BlendFactor::eOneMinusSrc1Color;
    case BlendFactor::Src1Alpha:
        return vk::BlendFactor::eSrc1Alpha;
    case BlendFactor::OneMinusSrc1Alpha:
        return vk::BlendFactor::eOneMinusSrc1Alpha;
    }
    return vk::BlendFactor(-1);
}
static inline auto ConvertToVk(const BlendOp& a_BlendOp) {
    switch (a_BlendOp)
    {
    case BlendOp::Add:
        return vk::BlendOp::eAdd;
    case BlendOp::Subtract:
        return vk::BlendOp::eSubtract;
    case BlendOp::ReverseSubtract:
        return vk::BlendOp::eReverseSubtract;
    case BlendOp::Min:
        return vk::BlendOp::eMin;
    case BlendOp::Max:
        return vk::BlendOp::eMax;
    }
    return vk::BlendOp(-1);
}
static inline auto ConvertToVk(const LoadOp& a_LoadOp) {
    switch (a_LoadOp)
    {
    case LoadOp::DontCare:
        return vk::AttachmentLoadOp::eDontCare;
    case LoadOp::Load:
        return vk::AttachmentLoadOp::eLoad;
    case LoadOp::Clear:
        return vk::AttachmentLoadOp::eClear;
    }
    return vk::AttachmentLoadOp(-1);
}
static inline auto ConvertToVk(const StoreOp& a_StoreOp) {
    switch (a_StoreOp)
    {
    case StoreOp::DontCare:
        return vk::AttachmentStoreOp::eDontCare;
    case StoreOp::Store:
        return vk::AttachmentStoreOp::eStore;
    }
    return vk::AttachmentStoreOp(-1);
}
static inline auto ConvertToVk(const DescriptorType& a_Type) {
    switch (a_Type)
    {
    case DescriptorType::SampledImage:
        return vk::DescriptorType::eSampledImage;
    case DescriptorType::StorageImage:
        return vk::DescriptorType::eStorageImage;
    case DescriptorType::UniformTexelBuffer:
        return vk::DescriptorType::eUniformTexelBuffer;
    case DescriptorType::StorageTexelBuffer:
        return vk::DescriptorType::eStorageTexelBuffer;
    case DescriptorType::UniformBuffer:
        return vk::DescriptorType::eUniformBuffer;
    case DescriptorType::StorageBuffer:
        return vk::DescriptorType::eStorageBuffer;
    case DescriptorType::UniformBufferDynamic:
        return vk::DescriptorType::eUniformBufferDynamic;
    case DescriptorType::StorageBufferDynamic:
        return vk::DescriptorType::eStorageBufferDynamic;
    case DescriptorType::InputAttachment:
        return vk::DescriptorType::eSampler;
    case DescriptorType::InlineUniformBlock:
        return vk::DescriptorType::eSampler;
    case DescriptorType::AccelerationStructure:
        return vk::DescriptorType::eSampler;
    case DescriptorType::MutableValve:
        return vk::DescriptorType::eSampler;
    }
    return vk::DescriptorType(-1);
}
static inline auto ConvertToVk(const ColorSpace& a_ColorSpace) {
    switch (a_ColorSpace)
    {
    case ColorSpace::Linear:
        return vk::ColorSpaceKHR::eExtendedSrgbLinearEXT;
    case ColorSpace::sRGB:
        return vk::ColorSpaceKHR::eSrgbNonlinear;
    case ColorSpace::HDR10:
        return vk::ColorSpaceKHR::eHdr10St2084EXT;
    }
    return vk::ColorSpaceKHR(-1);
}
static inline auto ConvertToVk(const CommandBufferLevel& a_Level) {
    switch (a_Level)
    {
    case CommandBufferLevel::Primary:
        return vk::CommandBufferLevel::ePrimary;
    case CommandBufferLevel::Secondary:
        return vk::CommandBufferLevel::eSecondary;
    }
    return vk::CommandBufferLevel(-1);
}
static inline auto ConvertToVk(const Swizzle& a_Swizzle) {
    switch (a_Swizzle)
    {
    case Swizzle::Identity:
        return vk::ComponentSwizzle::eIdentity;
    case Swizzle::Zero:
        return vk::ComponentSwizzle::eZero;
    case Swizzle::One:
        return vk::ComponentSwizzle::eOne;
    case Swizzle::R:
        return vk::ComponentSwizzle::eR;
    case Swizzle::G:
        return vk::ComponentSwizzle::eG;
    case Swizzle::B:
        return vk::ComponentSwizzle::eB;
    case Swizzle::A:
        return vk::ComponentSwizzle::eA;
    }
    return vk::ComponentSwizzle(-1);
}
static inline auto ConvertToVk(const Format& a_Format) {
    switch (a_Format)
    {
    case Format::Uint8_Normalized_R:
        return vk::Format::eR8Unorm;
    case Format::Uint8_Normalized_RG:
        return vk::Format::eR8G8Unorm;
    case Format::Uint8_Normalized_RGB:
        return vk::Format::eR8G8B8Unorm;
    case Format::Uint8_Normalized_RGBA:
        return vk::Format::eR8G8B8A8Unorm;
    case Format::Uint8_Normalized_BGR:
        return vk::Format::eB8G8R8Unorm;
    case Format::Uint8_Normalized_BGRA:
        return vk::Format::eB8G8R8A8Unorm;
    case Format::Uint8_SBGR:
        return vk::Format::eB8G8R8Srgb;
    case Format::Uint8_SBGRA:
        return vk::Format::eB8G8R8A8Srgb;
    case Format::Int8_Normalized_R:
        return vk::Format::eR8Snorm;
    case Format::Int8_Normalized_RG:
        return vk::Format::eR8G8Snorm;
    case Format::Int8_Normalized_RGB:
        return vk::Format::eR8G8B8Snorm;
    case Format::Int8_Normalized_RGBA:
        return vk::Format::eR8G8B8A8Snorm;
    case Format::Uint8_R:
        return vk::Format::eR8Uint;
    case Format::Uint8_RG:
        return vk::Format::eR8G8Uint;
    case Format::Uint8_RGB:
        return vk::Format::eR8G8B8Uint;
    case Format::Uint8_RGBA:
        return vk::Format::eR8G8B8A8Uint;
    case Format::Int8_R:
        return vk::Format::eR8Sint;
    case Format::Int8_RG:
        return vk::Format::eR8G8Sint;
    case Format::Int8_RGB:
        return vk::Format::eR8G8B8Sint;
    case Format::Int8_RGBA:
        return vk::Format::eR8G8B8A8Sint;
    case Format::Uint16_Normalized_R:
        return vk::Format::eR16Unorm;
    case Format::Uint16_Normalized_RG:
        return vk::Format::eR16G16Unorm;
    case Format::Uint16_Normalized_RGB:
        return vk::Format::eR16G16B16Unorm;
    case Format::Uint16_Normalized_RGBA:
        return vk::Format::eR16G16B16A16Unorm;
    case Format::Int16_Normalized_R:
        return vk::Format::eR16Snorm;
    case Format::Int16_Normalized_RG:
        return vk::Format::eR16G16Snorm;
    case Format::Int16_Normalized_RGB:
        return vk::Format::eR16G16B16Snorm;
    case Format::Int16_Normalized_RGBA:
        return vk::Format::eR16G16B16A16Snorm;
    case Format::Uint16_R:
        return vk::Format::eR16Uint;
    case Format::Uint16_RG:
        return vk::Format::eR16G16Uint;
    case Format::Uint16_RGB:
        return vk::Format::eR16G16B16Uint;
    case Format::Uint16_RGBA:
        return vk::Format::eR16G16B16A16Uint;
    case Format::Int16_R:
        return vk::Format::eR16Sint;
    case Format::Int16_RG:
        return vk::Format::eR16G16Sint;
    case Format::Int16_RGB:
        return vk::Format::eR16G16B16Sint;
    case Format::Int16_RGBA:
        return vk::Format::eR16G16B16A16Sint;
    case Format::Uint32_R:
        return vk::Format::eR32Uint;
    case Format::Uint32_RG:
        return vk::Format::eR32G32Uint;
    case Format::Uint32_RGB:
        return vk::Format::eR32G32B32Uint;
    case Format::Uint32_RGBA:
        return vk::Format::eR32G32B32A32Uint;
    case Format::Int32_R:
        return vk::Format::eR32Sint;
    case Format::Int32_RG:
        return vk::Format::eR32G32Sint;
    case Format::Int32_RGB:
        return vk::Format::eR32G32B32Sint;
    case Format::Int32_RGBA:
        return vk::Format::eR32G32B32A32Sint;
    case Format::Float16_R:
        return vk::Format::eR16Sfloat;
    case Format::Float16_RG:
        return vk::Format::eR16G16Sfloat;
    case Format::Float16_RGB:
        return vk::Format::eR16G16B16Sfloat;
    case Format::Float16_RGBA:
        return vk::Format::eR16G16B16A16Sfloat;
    case Format::Float32_R:
        return vk::Format::eR32Sfloat;
    case Format::Float32_RG:
        return vk::Format::eR32G32Sfloat;
    case Format::Float32_RGB:
        return vk::Format::eR32G32B32Sfloat;
    case Format::Float32_RGBA:
        return vk::Format::eR32G32B32A32Sfloat;
    case Format::Uint16_Normalized_Depth:
        return vk::Format::eD16Unorm;
    case Format::Float32_Depth:
        return vk::Format::eD32Sfloat;
    case Format::Uint24_Normalized_Depth_Uint8_Stencil:
        return vk::Format::eD24UnormS8Uint;
    case Format::Float32_Normalized_Depth_Uint8_Stencil:
        return vk::Format::eD32SfloatS8Uint;
    case Format::Uint8_Stencil:
        return vk::Format::eS8Uint;
    case Format::S3TC_DXT5_RGBA:
        return vk::Format::eBc3UnormBlock;
    case Format::S3TC_DXT5_SRGBA:
        return vk::Format::eBc3SrgbBlock;
    }
    return vk::Format(-1);
}
static inline auto ConvertToVk(const ImageLayout& a_Layout) {
    switch (a_Layout)
    {
    case ImageLayout::Undefined:
        return vk::ImageLayout::eUndefined;
    case ImageLayout::General:
        return vk::ImageLayout::eGeneral;
    case ImageLayout::ColorAttachmentOptimal:
        return vk::ImageLayout::eColorAttachmentOptimal;
    case ImageLayout::DepthStencilAttachmentOptimal:
        return vk::ImageLayout::eDepthStencilAttachmentOptimal;
    case ImageLayout::DepthStencilReadOnlyOptimal:
        return vk::ImageLayout::eDepthStencilReadOnlyOptimal;
    case ImageLayout::ShaderReadOnlyOptimal:
        return vk::ImageLayout::eShaderReadOnlyOptimal;
    case ImageLayout::TransferSrcOptimal:
        return vk::ImageLayout::eTransferSrcOptimal;
    case ImageLayout::TransferDstOptimal:
        return vk::ImageLayout::eTransferDstOptimal;
    case ImageLayout::Preinitialized:
        return vk::ImageLayout::ePreinitialized;
    case ImageLayout::DepthReadOnlyStencilAttachmentOptimal:
        return vk::ImageLayout::eDepthReadOnlyStencilAttachmentOptimal;
    case ImageLayout::DepthAttachmentStencilReadOnlyOptimal:
        return vk::ImageLayout::eDepthAttachmentStencilReadOnlyOptimal;
    case ImageLayout::DepthAttachmentOptimal:
        return vk::ImageLayout::eDepthAttachmentOptimal;
    case ImageLayout::DepthReadOnlyOptimal:
        return vk::ImageLayout::eDepthReadOnlyOptimal;
    case ImageLayout::StencilAttachmentOptimal:
        return vk::ImageLayout::eStencilAttachmentOptimal;
    case ImageLayout::StencilReadOnlyOptimal:
        return vk::ImageLayout::eStencilReadOnlyOptimal;
    case ImageLayout::ReadOnlyOptimal:
        return vk::ImageLayout::eReadOnlyOptimal;
    case ImageLayout::AttachmentOptimal:
        return vk::ImageLayout::eAttachmentOptimal;
    case ImageLayout::PresentSrc:
        return vk::ImageLayout::ePresentSrcKHR;
    }
    return vk::ImageLayout(-1);
}
static inline auto ConvertToVk(const ImageType& a_Type) {
    switch (a_Type)
    {
    case ImageType::Image1D:
        return vk::ImageType::e1D;
    case ImageType::Image2D:
        return vk::ImageType::e2D;
    case ImageType::Image3D:
        return vk::ImageType::e3D;
    }
    return vk::ImageType(-1);
}
static inline auto ConvertToVk(const ImageViewType& a_Type) {
    switch (a_Type)
    {
    case ImageViewType::View1D:
        return vk::ImageViewType::e1D;
    case ImageViewType::View2D:
        return vk::ImageViewType::e2D;
    case ImageViewType::View3D:
        return vk::ImageViewType::e3D;
    }
    return vk::ImageViewType(-1);
}
static inline auto ConvertToVk(const PipelineBindingPoint& a_BindingPoint)
{
    switch (a_BindingPoint)
    {
    case PipelineBindingPoint::Graphics:
        return VK_PIPELINE_BIND_POINT_GRAPHICS;
    case PipelineBindingPoint::Compute:
        return VK_PIPELINE_BIND_POINT_COMPUTE;
    case PipelineBindingPoint::RayTracing:
        return VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR;
    }
    return VK_PIPELINE_BIND_POINT_MAX_ENUM;
}
static inline auto ConvertToVk(const SampleCount& a_Count) {
    switch (a_Count)
    {
    case SampleCount::Count1:
        return vk::SampleCountFlagBits::e1;
    case SampleCount::Count2:
        return vk::SampleCountFlagBits::e2;
    case SampleCount::Count4:
        return vk::SampleCountFlagBits::e4;
    case SampleCount::Count8:
        return vk::SampleCountFlagBits::e8;
    case SampleCount::Count16:
        return vk::SampleCountFlagBits::e16;
    case SampleCount::Count32:
        return vk::SampleCountFlagBits::e32;
    case SampleCount::Count64:
        return vk::SampleCountFlagBits::e64;
    }
    return vk::SampleCountFlagBits(-1);
}
static inline auto ConvertToVk(const SharingMode& a_SharingMode) {
    switch (a_SharingMode)
    {
    case SharingMode::Exclusive :
        return vk::SharingMode::eExclusive;
    case SharingMode::Concurrent :
        return vk::SharingMode::eConcurrent;
    }
    return vk::SharingMode(-1);
}
}