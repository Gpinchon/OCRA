#pragma once

#include <OCRA/Structs.hpp>

#include <VK/Enums.hpp>
#include <VK/Flags.hpp>

#undef near
#undef far

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
static inline auto ConvertToVk(const PipelineStencilOpState& a_State) {
    return vk::StencilOpState(
        ConvertToVk(a_State.failOp),
        ConvertToVk(a_State.passOp),
        ConvertToVk(a_State.depthFailOp),
        ConvertToVk(a_State.compareOp),
        a_State.compareMask);
}
static inline auto ConvertToVk(const ViewPort& a_Viewport) {
    return vk::Viewport(
        a_Viewport.rect.offset.x,
        a_Viewport.rect.offset.y,
        a_Viewport.rect.extent.width,
        a_Viewport.rect.extent.height,
        a_Viewport.depthRange.near,
        a_Viewport.depthRange.far);
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
template<VertexType Type>
constexpr static inline vk::Format ConvertToVk(const uint8_t&, const bool&);
template<>
constexpr static inline vk::Format ConvertToVk<VertexType::Float32>(const uint8_t& a_Size, const bool& a_Normalized) {
    switch (a_Size)
    {
    case 1:
        return vk::Format::eR32Sfloat;
    case 2:
        return vk::Format::eR32G32Sfloat;
    case 3:
        return vk::Format::eR32G32B32Sfloat;
    case 4:
        return vk::Format::eR32G32B32A32Sfloat;
    }
    return vk::Format(-1);
}
template<>
constexpr static inline vk::Format ConvertToVk<VertexType::Float16>(const uint8_t& a_Size, const bool& a_Normalized) {
    switch (a_Size)
    {
    case 1:
        return vk::Format::eR16Sfloat;
    case 2:
        return vk::Format::eR16G16Sfloat;
    case 3:
        return vk::Format::eR16G16B16Sfloat;
    case 4:
        return vk::Format::eR16G16B16A16Sfloat;
    }
    return vk::Format(-1);
}
template<>
constexpr static inline vk::Format ConvertToVk<VertexType::Int32>  (const uint8_t& a_Size, const bool& a_Normalized) {
    switch (a_Size)
    {
    case 1:
        return vk::Format::eR32Sint;
    case 2:
        return vk::Format::eR32G32Sint;
    case 3:
        return vk::Format::eR32G32B32Sint;
    case 4:
        return vk::Format::eR32G32B32A32Sint;
    }
    return vk::Format(-1);
}
template<>
constexpr static inline vk::Format ConvertToVk<VertexType::Uint32> (const uint8_t& a_Size, const bool& a_Normalized) {
    switch (a_Size)
    {
    case 1:
        return vk::Format::eR32Uint;
    case 2:
        return vk::Format::eR32G32Uint;
    case 3:
        return vk::Format::eR32G32B32Uint;
    case 4:
        return vk::Format::eR32G32B32A32Uint;
    }
    return vk::Format(-1);
}
template<>
constexpr static inline vk::Format ConvertToVk<VertexType::Int16>  (const uint8_t& a_Size, const bool& a_Normalized) {
    switch (a_Size)
    {
    case 1:
        return a_Normalized ? vk::Format::eR16Snorm : vk::Format::eR16Sint;
    case 2:
        return a_Normalized ? vk::Format::eR16G16Snorm : vk::Format::eR16G16Sint;
    case 3:
        return a_Normalized ? vk::Format::eR16G16B16Snorm : vk::Format::eR16G16B16Sint;
    case 4:
        return a_Normalized ? vk::Format::eR16G16B16A16Snorm : vk::Format::eR16G16B16A16Sint;
    }
    return vk::Format(-1);
}
template<>
constexpr static inline vk::Format ConvertToVk<VertexType::Uint16> (const uint8_t& a_Size, const bool& a_Normalized) {
    switch (a_Size)
    {
    case 1:
        return a_Normalized ? vk::Format::eR16Unorm : vk::Format::eR16Uint;
    case 2:
        return a_Normalized ? vk::Format::eR16G16Unorm : vk::Format::eR16G16Uint;
    case 3:
        return a_Normalized ? vk::Format::eR16G16B16Unorm : vk::Format::eR16G16B16Uint;
    case 4:
        return a_Normalized ? vk::Format::eR16G16B16A16Unorm : vk::Format::eR16G16B16A16Uint;
    }
    return vk::Format(-1);
}
template<>
constexpr static inline vk::Format ConvertToVk<VertexType::Int8>   (const uint8_t& a_Size, const bool& a_Normalized) {
    switch (a_Size)
    {
    case 1:
        return a_Normalized ? vk::Format::eR8Snorm : vk::Format::eR8Sint;
    case 2:
        return a_Normalized ? vk::Format::eR8G8Snorm : vk::Format::eR8G8Sint;
    case 3:
        return a_Normalized ? vk::Format::eR8G8B8Snorm : vk::Format::eR8G8B8Sint;
    case 4:
        return a_Normalized ? vk::Format::eR8G8B8A8Snorm : vk::Format::eR8G8B8A8Sint;
    }
    return vk::Format(-1);
}
template<>
constexpr static inline vk::Format ConvertToVk<VertexType::Uint8>  (const uint8_t& a_Size, const bool& a_Normalized) {
    switch (a_Size)
    {
    case 1:
        return a_Normalized ? vk::Format::eR8Unorm : vk::Format::eR8Uint;
    case 2:
        return a_Normalized ? vk::Format::eR8G8Unorm : vk::Format::eR8G8Uint;
    case 3:
        return a_Normalized ? vk::Format::eR8G8B8Unorm : vk::Format::eR8G8B8Uint;
    case 4:
        return a_Normalized ? vk::Format::eR8G8B8A8Unorm : vk::Format::eR8G8B8A8Uint;
    }
    return vk::Format(-1);
}
constexpr static inline vk::Format ConvertToVk(const VertexAttributeFormat& a_Format) {
    switch (a_Format.type)
    {
    case VertexType::Float32:
        return ConvertToVk<VertexType::Float32>(a_Format.size, a_Format.normalized);
    case VertexType::Float16:
        return ConvertToVk<VertexType::Float16>(a_Format.size, a_Format.normalized);
    case VertexType::Int32:
        return ConvertToVk<VertexType::Int32>  (a_Format.size, a_Format.normalized);
    case VertexType::Uint32:
        return ConvertToVk<VertexType::Uint32> (a_Format.size, a_Format.normalized);
    case VertexType::Int16:
        return ConvertToVk<VertexType::Int16>  (a_Format.size, a_Format.normalized);
    case VertexType::Uint16:
        return ConvertToVk<VertexType::Uint16> (a_Format.size, a_Format.normalized);
    case VertexType::Int8:
        return ConvertToVk<VertexType::Int8>   (a_Format.size, a_Format.normalized);
    case VertexType::Uint8:
        return ConvertToVk<VertexType::Uint8>  (a_Format.size, a_Format.normalized);
    }
    return vk::Format(-1);
}
}