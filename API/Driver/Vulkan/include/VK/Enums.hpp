/**
 * @brief this header contains functions to convert OCRA enums to VK enums
 */
#pragma once

#include <OCRA/Enums.hpp>

#include <vulkan/vulkan.h>

namespace OCRA
{
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
        break;
    case Format::Uint16_Normalized_RG:
        break;
    case Format::Uint16_Normalized_RGB:
        break;
    case Format::Uint16_Normalized_RGBA:
        break;
    case Format::Int16_Normalized_R:
        break;
    case Format::Int16_Normalized_RG:
        break;
    case Format::Int16_Normalized_RGB:
        break;
    case Format::Int16_Normalized_RGBA:
        break;
    case Format::Uint16_R:
        break;
    case Format::Uint16_RG:
        break;
    case Format::Uint16_RGB:
        break;
    case Format::Uint16_RGBA:
        break;
    case Format::Int16_R:
        break;
    case Format::Int16_RG:
        break;
    case Format::Int16_RGB:
        break;
    case Format::Int16_RGBA:
        break;
    case Format::Uint32_R:
        break;
    case Format::Uint32_RG:
        break;
    case Format::Uint32_RGB:
        break;
    case Format::Uint32_RGBA:
        break;
    case Format::Int32_R:
        break;
    case Format::Int32_RG:
        break;
    case Format::Int32_RGB:
        break;
    case Format::Int32_RGBA:
        break;
    case Format::Float16_R:
        break;
    case Format::Float16_RG:
        break;
    case Format::Float16_RGB:
        break;
    case Format::Float16_RGBA:
        break;
    case Format::Float32_R:
        break;
    case Format::Float32_RG:
        break;
    case Format::Float32_RGB:
        break;
    case Format::Float32_RGBA:
        break;
    case Format::Uint16_Normalized_Depth:
        break;
    case Format::Float32_Depth:
        break;
    case Format::Uint24_Normalized_Depth_Uint8_Stencil:
        break;
    case Format::Float32_Normalized_Depth_Uint8_Stencil:
        break;
    case Format::Uint8_Stencil:
        break;
    case Format::S3TC_DXT5_RGBA:
        break;
    case Format::S3TC_DXT5_SRGBA:
        break;
    case Format::MaxValue:
        break;
    }
    return VK_FORMAT_MAX_ENUM;
}
}