#pragma once

#include <OCRA/Image/Image.hpp>

#include <dxgi1_3.h>

namespace OCRA
{
static inline auto GetDXFormat(const Image::Format& a_Format, bool a_SRGB)
{
    switch (a_Format)
    {
    case Image::Format::Uint8_Normalized_R:
        return DXGI_FORMAT_R8_UNORM;
    case Image::Format::Uint8_Normalized_RG:
        return DXGI_FORMAT_R8G8_UNORM;
    case Image::Format::Uint8_Normalized_RGBA:
        return a_SRGB ? DXGI_FORMAT_R8G8B8A8_UNORM_SRGB : DXGI_FORMAT_R8G8B8A8_UNORM;
    case Image::Format::Int8_Normalized_R:
        return DXGI_FORMAT_R8_SNORM;
    case Image::Format::Int8_Normalized_RG:
        return DXGI_FORMAT_R8G8_SNORM;
    case Image::Format::Int8_Normalized_RGBA:
        return DXGI_FORMAT_R8G8B8A8_SNORM;
    case Image::Format::Uint8_R:
        return DXGI_FORMAT_R8_UINT;
    case Image::Format::Uint8_RG:
        return DXGI_FORMAT_R8G8_UINT;
    case Image::Format::Uint8_RGBA:
        return DXGI_FORMAT_R8G8B8A8_UINT;
    case Image::Format::Int8_R:
        return DXGI_FORMAT_R8_SINT;
    case Image::Format::Int8_RG:
        return DXGI_FORMAT_R8G8_SINT;
    case Image::Format::Int8_RGBA:
        return DXGI_FORMAT_R8G8B8A8_SINT;
    case Image::Format::Uint16_Normalized_R:
        return DXGI_FORMAT_R16_UNORM;
    case Image::Format::Uint16_Normalized_RG:
        return DXGI_FORMAT_R16G16_UNORM;
    case Image::Format::Uint16_Normalized_RGBA:
        return DXGI_FORMAT_R16G16B16A16_UNORM;
    case Image::Format::Int16_Normalized_R:
        return DXGI_FORMAT_R16_SNORM;
    case Image::Format::Int16_Normalized_RG:
        return DXGI_FORMAT_R16G16_SNORM;
    case Image::Format::Int16_Normalized_RGBA:
        return DXGI_FORMAT_R16G16B16A16_SNORM;
    case Image::Format::Uint16_R:
        return DXGI_FORMAT_R16_UINT;
    case Image::Format::Uint16_RG:
        return DXGI_FORMAT_R16G16_UINT;
    case Image::Format::Uint16_RGBA:
        return DXGI_FORMAT_R16G16B16A16_UINT;
    case Image::Format::Int16_R:
        return DXGI_FORMAT_R16_SINT;
    case Image::Format::Int16_RG:
        return DXGI_FORMAT_R16G16_SINT;
    case Image::Format::Int16_RGBA:
        return DXGI_FORMAT_R16G16B16A16_SINT;
    case Image::Format::Uint32_R:
        return DXGI_FORMAT_R32_UINT;
    case Image::Format::Uint32_RG:
        return DXGI_FORMAT_R32G32_UINT;
    case Image::Format::Uint32_RGB:
        return DXGI_FORMAT_R32G32B32_UINT;
    case Image::Format::Uint32_RGBA:
        return DXGI_FORMAT_R32G32B32A32_UINT;
    case Image::Format::Int32_R:
        return DXGI_FORMAT_R32_SINT;
    case Image::Format::Int32_RG:
        return DXGI_FORMAT_R32G32_SINT;
    case Image::Format::Int32_RGB:
        return DXGI_FORMAT_R32G32B32_SINT;
    case Image::Format::Int32_RGBA:
        return DXGI_FORMAT_R32G32B32A32_SINT;
    case Image::Format::Float16_R:
        return DXGI_FORMAT_R16_FLOAT;
    case Image::Format::Float16_RG:
        return DXGI_FORMAT_R16G16_FLOAT;
    case Image::Format::Float16_RGBA:
        return DXGI_FORMAT_R16G16B16A16_FLOAT;
    case Image::Format::Float32_R:
        return DXGI_FORMAT_R32_FLOAT;
    case Image::Format::Float32_RG:
        return DXGI_FORMAT_R32G32_FLOAT;
    case Image::Format::Float32_RGB:
        return DXGI_FORMAT_R32G32B32_FLOAT;
    case Image::Format::Float32_RGBA:
        return DXGI_FORMAT_R32G32B32A32_FLOAT;
    case Image::Format::Uint16_Normalized_Depth:
        return DXGI_FORMAT_D16_UNORM;
    case Image::Format::Float32_Depth:
        return DXGI_FORMAT_D32_FLOAT;
    case Image::Format::Uint24_Normalized_Depth_Uint8_Stencil:
        return DXGI_FORMAT_D24_UNORM_S8_UINT;
    case Image::Format::Float32_Normalized_Depth_Uint8_Stencil:
        return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
    default:
        throw std::runtime_error("Incompatible Image Format");
    }
}
}