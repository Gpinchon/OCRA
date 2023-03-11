#pragma once

#include <VK/Enums.hpp>
#include <VK/Flags.hpp>

#include <OCRA/Structs.hpp>

namespace OCRA
{
static inline vk::ClearColorValue ConvertToVk(const ColorValue& a_ClearValue) {
    return reinterpret_cast<const vk::ClearColorValue&>(a_ClearValue);
}
static inline vk::ClearDepthStencilValue ConvertToVk(const DepthStencilValue& a_ClearValue) {
    return reinterpret_cast<const vk::ClearDepthStencilValue&>(a_ClearValue);
}
static inline vk::ClearValue ConvertToVk(const ClearValue& a_ClearValue) {
    return reinterpret_cast<const vk::ClearValue&>(a_ClearValue);
}
static inline vk::ComponentMapping ConvertToVk(const ComponentMapping& a_Mapping) {
    return vk::ComponentMapping(
        ConvertToVk(a_Mapping.r),
        ConvertToVk(a_Mapping.g),
        ConvertToVk(a_Mapping.b),
        ConvertToVk(a_Mapping.a)
    );
}
template <typename T>
static inline auto ConvertToVk(const Extent<2, T>& a_Extent) {
    return vk::Extent2D(
        a_Extent.width,
        a_Extent.height
    );
}
template <typename T>
static inline auto ConvertToVk(const Extent<3, T>& a_Extent) {
    return vk::Extent3D(
        a_Extent.width,
        a_Extent.height,
        a_Extent.depth
    );
}
static inline auto ConvertToVk(const Offset2D& a_Offset) {
    return vk::Offset2D(
        a_Offset.x,
        a_Offset.y
    );
}
static inline auto ConvertToVk(const Offset3D& a_Offset) {
    return vk::Offset3D(
        a_Offset.x,
        a_Offset.y,
        a_Offset.z
    );
}
static inline auto ConvertToVk(const Rect2D& a_Rect2D) {
    return vk::Rect2D(
        ConvertToVk(a_Rect2D.offset),
        ConvertToVk(a_Rect2D.extent)
    );
}
static inline auto ConvertToVk(const ImageSubresourceLayers& a_Layers) {
    return vk::ImageSubresourceLayers(
        ConvertToVk(a_Layers.aspects),
        a_Layers.mipLevel,
        a_Layers.baseArrayLayer, a_Layers.layerCount
    );
}
static inline auto ConvertToVk(const ImageSubresourceRange& a_Range) {
    return vk::ImageSubresourceRange(
        ConvertToVk(a_Range.aspects),
        a_Range.baseMipLevel, a_Range.levelCount,
        a_Range.baseArrayLayer, a_Range.layerCount
    );
}
static inline auto ConvertToVk(const PipelineColorBlendAttachmentState& a_State) {
    return vk::PipelineColorBlendAttachmentState(
        a_State.enable,
        ConvertToVk(a_State.srcColorBlendFactor), ConvertToVk(a_State.dstColorBlendFactor), ConvertToVk(a_State.colorBlendOperation),
        ConvertToVk(a_State.srcAlphaBlendFactor), ConvertToVk(a_State.dstAlphaBlendFactor), ConvertToVk(a_State.alphaBlendOperation),
        ConvertToVk(a_State.colorMask));
}
}