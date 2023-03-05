/**
 * @brief this header contains functions to convert OCRA enums to VK enums
 */
#pragma once

#include <OCRA/Enums.hpp>

#include <vulkan/vulkan.h>

namespace OCRA
{
static inline auto GetVkImageLayout(const ImageLayout& a_Layout) {
    switch (a_Layout)
    {
    case OCRA::ImageLayout::Undefined:
        return VK_IMAGE_LAYOUT_UNDEFINED;
    case OCRA::ImageLayout::General:
        return VK_IMAGE_LAYOUT_GENERAL;
    case OCRA::ImageLayout::ColorAttachmentOptimal:
        return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    case OCRA::ImageLayout::DepthStencilAttachmentOptimal:
        return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    case OCRA::ImageLayout::DepthStencilReadOnlyOptimal:
        return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
    case OCRA::ImageLayout::ShaderReadOnlyOptimal:
        return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    case OCRA::ImageLayout::TransferSrcOptimal:
        return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    case OCRA::ImageLayout::TransferDstOptimal:
        return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    case OCRA::ImageLayout::Preinitialized:
        return VK_IMAGE_LAYOUT_PREINITIALIZED;
    case OCRA::ImageLayout::DepthReadOnlyStencilAttachmentOptimal:
        return VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL;
    case OCRA::ImageLayout::DepthAttachmentStencilReadOnlyOptimal:
        return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL;
    case OCRA::ImageLayout::DepthAttachmentOptimal:
        return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
    case OCRA::ImageLayout::DepthReadOnlyOptimal:
        return VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL;
    case OCRA::ImageLayout::StencilAttachmentOptimal:
        return VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL;
    case OCRA::ImageLayout::StencilReadOnlyOptimal:
        return VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL;
    case OCRA::ImageLayout::ReadOnlyOptimal:
        return VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL;
    case OCRA::ImageLayout::AttachmentOptimal:
        return VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
    case OCRA::ImageLayout::PresentSrc:
        return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    }
    return VK_IMAGE_LAYOUT_MAX_ENUM;
}
static inline auto GetVkDescriptorType(const DescriptorType& a_Type) {
    switch (a_Type)
    {
    case DescriptorType::SampledImage:
        return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    case DescriptorType::StorageImage:
        return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    case DescriptorType::UniformTexelBuffer:
        return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
    case DescriptorType::StorageTexelBuffer:
        return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
    case DescriptorType::UniformBuffer:
        return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    case DescriptorType::StorageBuffer:
        return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    case DescriptorType::UniformBufferDynamic:
        return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    case DescriptorType::StorageBufferDynamic:
        return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
    case DescriptorType::InputAttachment:
        return VK_DESCRIPTOR_TYPE_SAMPLER;
    case DescriptorType::InlineUniformBlock:
        return VK_DESCRIPTOR_TYPE_SAMPLER;
    case DescriptorType::AccelerationStructure:
        return VK_DESCRIPTOR_TYPE_SAMPLER;
    case DescriptorType::MutableValve:
        return VK_DESCRIPTOR_TYPE_SAMPLER;
    }
    return VK_DESCRIPTOR_TYPE_MAX_ENUM;
}
static inline auto GetVkSharingMode(const SharingMode& a_SharingMode) {
    switch (a_SharingMode)
    {
    case SharingMode::Exclusive :
        return VK_SHARING_MODE_EXCLUSIVE;
    case SharingMode::Concurrent :
        return VK_SHARING_MODE_EXCLUSIVE;
    }
    return VK_SHARING_MODE_MAX_ENUM;
}
static inline auto GetVkCommandBufferLevel(const CommandBufferLevel& a_Level) {
    switch (a_Level)
    {
    case CommandBufferLevel::Primary:
        return VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    case CommandBufferLevel::Secondary:
        return VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    }
    return VK_COMMAND_BUFFER_LEVEL_MAX_ENUM;
}
static inline auto GetVkColorSpace(const ColorSpace& a_ColorSpace) {
    switch (a_ColorSpace)
    {
    case ColorSpace::Linear:
        return VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT;
    case ColorSpace::sRGB:
        return VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    case ColorSpace::HDR10:
        return VK_COLOR_SPACE_HDR10_ST2084_EXT;
    }
    return VK_COLOR_SPACE_MAX_ENUM_KHR;
}
static inline auto GetOCColorSpace(const VkColorSpaceKHR& a_ColorSpace) {
    switch (a_ColorSpace)
    {
    case VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT:
        return ColorSpace::Linear;
    case VK_COLOR_SPACE_SRGB_NONLINEAR_KHR:
        return ColorSpace::sRGB;
    case VK_COLOR_SPACE_HDR10_ST2084_EXT:
        return ColorSpace::HDR10;
    }
    return ColorSpace::MaxValue;
}
static inline auto GetVkFormat(const Format& a_Format) {
    switch (a_Format)
    {
    case Format::Uint8_Normalized_R:
        return VK_FORMAT_R8_UNORM;
    case Format::Uint8_Normalized_RG:
        return VK_FORMAT_R8G8_UNORM;
    case Format::Uint8_Normalized_RGB:
        return VK_FORMAT_R8G8B8_UNORM;
    case Format::Uint8_Normalized_RGBA:
        return VK_FORMAT_R8G8B8A8_UNORM;
    case Format::Uint8_Normalized_BGR:
        return VK_FORMAT_B8G8R8_UNORM;
    case Format::Uint8_Normalized_BGRA:
        return VK_FORMAT_B8G8R8A8_UNORM;
    case Format::Uint8_SBGR:
        return VK_FORMAT_B8G8R8_SRGB;
    case Format::Uint8_SBGRA:
        return VK_FORMAT_B8G8R8A8_SRGB;
    case Format::Int8_Normalized_R:
        return VK_FORMAT_R8_SNORM;
    case Format::Int8_Normalized_RG:
        return VK_FORMAT_R8G8_SNORM;
    case Format::Int8_Normalized_RGB:
        return VK_FORMAT_R8G8B8_SNORM;
    case Format::Int8_Normalized_RGBA:
        return VK_FORMAT_R8G8B8A8_SNORM;
    case Format::Uint8_R:
        return VK_FORMAT_R8_UINT;
    case Format::Uint8_RG:
        return VK_FORMAT_R8G8_UINT;
    case Format::Uint8_RGB:
        return VK_FORMAT_R8G8B8_UINT;
    case Format::Uint8_RGBA:
        return VK_FORMAT_R8G8B8A8_UINT;
    case Format::Int8_R:
        return VK_FORMAT_R8_SINT;
    case Format::Int8_RG:
        return VK_FORMAT_R8G8_SINT;
    case Format::Int8_RGB:
        return VK_FORMAT_R8G8B8_SINT;
    case Format::Int8_RGBA:
        return VK_FORMAT_R8G8B8A8_SINT;
    case Format::Uint16_Normalized_R:
        return VK_FORMAT_R16_UNORM;
    case Format::Uint16_Normalized_RG:
        return VK_FORMAT_R16G16_UNORM;
    case Format::Uint16_Normalized_RGB:
        return VK_FORMAT_R16G16B16_UNORM;
    case Format::Uint16_Normalized_RGBA:
        return VK_FORMAT_R16G16B16A16_UNORM;
    case Format::Int16_Normalized_R:
        return VK_FORMAT_R16_SNORM;
    case Format::Int16_Normalized_RG:
        return VK_FORMAT_R16G16_SNORM;
    case Format::Int16_Normalized_RGB:
        return VK_FORMAT_R16G16B16_SNORM;
    case Format::Int16_Normalized_RGBA:
        return VK_FORMAT_R16G16B16A16_SNORM;
    case Format::Uint16_R:
        return VK_FORMAT_R16_UINT;
    case Format::Uint16_RG:
        return VK_FORMAT_R16G16_UINT;
    case Format::Uint16_RGB:
        return VK_FORMAT_R16G16B16_UINT;
    case Format::Uint16_RGBA:
        return VK_FORMAT_R16G16B16A16_UINT;
    case Format::Int16_R:
        return VK_FORMAT_R16_SINT;
    case Format::Int16_RG:
        return VK_FORMAT_R16G16_SINT;
    case Format::Int16_RGB:
        return VK_FORMAT_R16G16B16_SINT;
    case Format::Int16_RGBA:
        return VK_FORMAT_R16G16B16A16_SINT;
    case Format::Uint32_R:
        return VK_FORMAT_R32_UINT;
    case Format::Uint32_RG:
        return VK_FORMAT_R32G32_UINT;
    case Format::Uint32_RGB:
        return VK_FORMAT_R32G32B32_UINT;
    case Format::Uint32_RGBA:
        return VK_FORMAT_R32G32B32A32_UINT;
    case Format::Int32_R:
        return VK_FORMAT_R32_SINT;
    case Format::Int32_RG:
        return VK_FORMAT_R32G32_SINT;
    case Format::Int32_RGB:
        return VK_FORMAT_R32G32B32_SINT;
    case Format::Int32_RGBA:
        return VK_FORMAT_R32G32B32A32_SINT;
    case Format::Float16_R:
        return VK_FORMAT_R16_SFLOAT;
    case Format::Float16_RG:
        return VK_FORMAT_R16G16_SFLOAT;
    case Format::Float16_RGB:
        return VK_FORMAT_R16G16B16_SFLOAT;
    case Format::Float16_RGBA:
        return VK_FORMAT_R16G16B16A16_SFLOAT;
    case Format::Float32_R:
        return VK_FORMAT_R32_SFLOAT;
    case Format::Float32_RG:
        return VK_FORMAT_R32G32_SFLOAT;
    case Format::Float32_RGB:
        return VK_FORMAT_R32G32B32_SFLOAT;
    case Format::Float32_RGBA:
        return VK_FORMAT_R32G32B32A32_SFLOAT;
    case Format::Uint16_Normalized_Depth:
        return VK_FORMAT_D16_UNORM;
    case Format::Float32_Depth:
        return VK_FORMAT_D32_SFLOAT;
    case Format::Uint24_Normalized_Depth_Uint8_Stencil:
        return VK_FORMAT_D24_UNORM_S8_UINT;
    case Format::Float32_Normalized_Depth_Uint8_Stencil:
        return VK_FORMAT_D32_SFLOAT_S8_UINT;
    case Format::Uint8_Stencil:
        return VK_FORMAT_S8_UINT;
    case Format::S3TC_DXT5_RGBA:
        return VK_FORMAT_BC3_UNORM_BLOCK;
    case Format::S3TC_DXT5_SRGBA:
        return VK_FORMAT_BC3_SRGB_BLOCK;
    }
    return VK_FORMAT_MAX_ENUM;
}
static inline auto GetOCFormat(const VkFormat& a_Format) {
    switch (a_Format)
    {
    case VK_FORMAT_R8_UNORM:
        return Format::Uint8_Normalized_R;
    case VK_FORMAT_R8G8_UNORM:
        return Format::Uint8_Normalized_RG;
    case VK_FORMAT_R8G8B8_UNORM:
        return Format::Uint8_Normalized_RGB;
    case VK_FORMAT_R8G8B8A8_UNORM:
        return Format::Uint8_Normalized_RGBA;
    case VK_FORMAT_B8G8R8_UNORM:
        return Format::Uint8_Normalized_BGR;
    case VK_FORMAT_B8G8R8A8_UNORM:
        return Format::Uint8_Normalized_BGRA;
    case VK_FORMAT_B8G8R8_SRGB:
        return Format::Uint8_SBGR;
    case VK_FORMAT_B8G8R8A8_SRGB:
        return Format::Uint8_SBGRA;
    case VK_FORMAT_R8_SNORM:
        return Format::Int8_Normalized_R;
    case VK_FORMAT_R8G8_SNORM:
        return Format::Int8_Normalized_RG;
    case VK_FORMAT_R8G8B8_SNORM:
        return Format::Int8_Normalized_RGB;
    case VK_FORMAT_R8G8B8A8_SNORM:
        return Format::Int8_Normalized_RGBA;
    case VK_FORMAT_R8_UINT:
        return Format::Uint8_R;
    case VK_FORMAT_R8G8_UINT:
        return Format::Uint8_RG;
    case VK_FORMAT_R8G8B8_UINT:
        return Format::Uint8_RGB;
    case VK_FORMAT_R8G8B8A8_UINT:
        return Format::Uint8_RGBA;
    case VK_FORMAT_R8_SINT:
        return Format::Int8_R;
    case VK_FORMAT_R8G8_SINT:
        return Format::Int8_RG;
    case VK_FORMAT_R8G8B8_SINT:
        return Format::Int8_RGB;
    case VK_FORMAT_R8G8B8A8_SINT:
        return Format::Int8_RGBA;
    case VK_FORMAT_R16_UNORM:
        return Format::Uint16_Normalized_R;
    case VK_FORMAT_R16G16_UNORM:
        return Format::Uint16_Normalized_RG;
    case VK_FORMAT_R16G16B16_UNORM:
        return Format::Uint16_Normalized_RGB;
    case VK_FORMAT_R16G16B16A16_UNORM:
        return Format::Uint16_Normalized_RGBA;
    case VK_FORMAT_R16_SNORM:
        return Format::Int16_Normalized_R;
    case VK_FORMAT_R16G16_SNORM:
        return Format::Int16_Normalized_RG;
    case VK_FORMAT_R16G16B16_SNORM:
        return Format::Int16_Normalized_RGB;
    case VK_FORMAT_R16G16B16A16_SNORM:
        return Format::Int16_Normalized_RGBA;
    case VK_FORMAT_R16_UINT:
        return Format::Uint16_R;
    case VK_FORMAT_R16G16_UINT:
        return Format::Uint16_RG;
    case VK_FORMAT_R16G16B16_UINT:
        return Format::Uint16_RGB;
    case VK_FORMAT_R16G16B16A16_UINT:
        return Format::Uint16_RGBA;
    case VK_FORMAT_R16_SINT:
        return Format::Int16_R;
    case VK_FORMAT_R16G16_SINT:
        return Format::Int16_RG;
    case VK_FORMAT_R16G16B16_SINT:
        return Format::Int16_RGB;
    case VK_FORMAT_R16G16B16A16_SINT:
        return Format::Int16_RGBA;
    case VK_FORMAT_R32_UINT:
        return Format::Uint32_R;
    case VK_FORMAT_R32G32_UINT:
        return Format::Uint32_RG;
    case VK_FORMAT_R32G32B32_UINT:
        return Format::Uint32_RGB;
    case VK_FORMAT_R32G32B32A32_UINT:
        return Format::Uint32_RGBA;
    case VK_FORMAT_R32_SINT:
        return Format::Int32_R;
    case VK_FORMAT_R32G32_SINT:
        return Format::Int32_RG;
    case VK_FORMAT_R32G32B32_SINT:
        return Format::Int32_RGB;
    case VK_FORMAT_R32G32B32A32_SINT:
        return Format::Int32_RGBA;
    case VK_FORMAT_R16_SFLOAT:
        return Format::Float16_R;
    case VK_FORMAT_R16G16_SFLOAT:
        return Format::Float16_RG;
    case VK_FORMAT_R16G16B16_SFLOAT:
        return Format::Float16_RGB;
    case VK_FORMAT_R16G16B16A16_SFLOAT:
        return Format::Float16_RGBA;
    case VK_FORMAT_R32_SFLOAT:
        return Format::Float32_R;
    case VK_FORMAT_R32G32_SFLOAT:
        return Format::Float32_RG;
    case VK_FORMAT_R32G32B32_SFLOAT:
        return Format::Float32_RGB;
    case VK_FORMAT_R32G32B32A32_SFLOAT:
        return Format::Float32_RGBA;
    case VK_FORMAT_D16_UNORM:
        return Format::Uint16_Normalized_Depth;
    case VK_FORMAT_D32_SFLOAT:
        return Format::Float32_Depth;
    case VK_FORMAT_D24_UNORM_S8_UINT:
        return Format::Uint24_Normalized_Depth_Uint8_Stencil;
    case VK_FORMAT_D32_SFLOAT_S8_UINT:
        return Format::Float32_Normalized_Depth_Uint8_Stencil;
    case VK_FORMAT_S8_UINT:
        return Format::Uint8_Stencil;
    case VK_FORMAT_BC3_UNORM_BLOCK:
        return Format::S3TC_DXT5_RGBA;
    case VK_FORMAT_BC3_SRGB_BLOCK:
        return Format::S3TC_DXT5_SRGBA;
    case VkFormat::VK_FORMAT_A2B10G10R10_UNORM_PACK32:
        return Format::Normalized_BGR10_A2;
    case VkFormat::VK_FORMAT_A2B10G10R10_UINT_PACK32:
        return Format::BGR10_A2;
    }
    return Format::MaxValue;
}

}