#pragma once

#include <VK/Flags.hpp>

#include <OCRA/Structs.hpp>

namespace OCRA
{
static inline VkClearColorValue GetVkClearColorValue(const ColorValue& a_ClearValue) {
    return reinterpret_cast<const VkClearColorValue&>(a_ClearValue);
}
static inline VkClearDepthStencilValue GetVkClearDepthStencilValue(const DepthStencilValue& a_ClearValue) {
    return reinterpret_cast<const VkClearDepthStencilValue&>(a_ClearValue);
}
static inline VkClearValue GetVkClearValue(const ClearValue& a_ClearValue) {
    return reinterpret_cast<const VkClearValue&>(a_ClearValue);
}
template <typename T>
static inline auto GetVkExtent2D(const Extent<2, T>& a_Extent) {
    return VkExtent2D{
        uint32_t(a_Extent.width),
        uint32_t(a_Extent.height)
    };
}
template <typename T>
static inline auto GetVkExtent3D(const Extent<3, T>& a_Extent) {
    return VkExtent3D{
        uint32_t(a_Extent.width),
        uint32_t(a_Extent.height),
        uint32_t(a_Extent.depth)
    };
}
static inline auto GetVkOffset2D(const Offset2D& a_Offset) {
    return VkOffset2D{
        int32_t(a_Offset.x),
        int32_t(a_Offset.y)
    };
}
static inline auto GetVkOffset3D(const Offset3D& a_Offset) {
    return VkOffset3D{
        int32_t(a_Offset.x),
        int32_t(a_Offset.y),
        int32_t(a_Offset.z)
    };
}
static inline auto GetVkRect2D(const Rect2D& a_Rect2D) {
    return VkRect2D{
        GetVkOffset2D(a_Rect2D.offset),
        GetVkExtent2D(a_Rect2D.extent)
    };
}
static inline auto GetVkImageSubresourceLayers(const ImageSubresourceLayers& a_Layers) {
    return vk::ImageSubresourceLayers{
        GetVkImageAspectFlags(a_Layers.aspects),
        a_Layers.mipLevel,
        a_Layers.baseArrayLayer, a_Layers.layerCount
    };
}
static inline auto GetVkImageSubresourceRange(const ImageSubresourceRange& a_Range) {
    return vk::ImageSubresourceRange{
        GetVkImageAspectFlags(a_Range.aspects),
        a_Range.baseMipLevel, a_Range.levelCount,
        a_Range.baseArrayLayer, a_Range.layerCount
    };
}
}