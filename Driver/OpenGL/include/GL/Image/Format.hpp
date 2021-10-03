/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:31:35
*/
#pragma once

#include <Image/Image.hpp>

#include <stdexcept>

#include <GL/glew.h>

namespace OCRA::Image {
inline auto GetGLFormat(const Format& format)
{
    switch (format) {
    case Format::Uint8_NormalizedR:
        return GL_R8;
    case Format::Uint8_NormalizedRG:
        return GL_RG8;
    case Format::Uint8_NormalizedRGB:
        return GL_RGB8;
    case Format::Uint8_NormalizedRGBA:
        return GL_RGBA8;
    case Format::Int8_NormalizedR:
        return GL_R8_SNORM;
    case Format::Int8_NormalizedRG:
        return GL_RG8_SNORM;
    case Format::Int8_NormalizedRGB:
        return GL_RGB8_SNORM;
    case Format::Int8_NormalizedRGBA:
        return GL_RGBA8_SNORM;
    case Format::Uint8_R:
        return GL_R8UI;
    case Format::Uint8_RG:
        return GL_RG8UI;
    case Format::Uint8_RGB:
        return GL_RGB8UI;
    case Format::Uint8_RGBA:
        return GL_RGBA8UI;
    case Format::Int8_R:
        return GL_R8I;
    case Format::Int8_RG:
        return GL_RG8I;
    case Format::Int8_RGB:
        return GL_RGB8I;
    case Format::Int8_RGBA:
        return GL_RGBA8I;
    case Format::Uint16_NormalizedR:
        return GL_R16;
    case Format::Uint16_NormalizedRG:
        return GL_RG16;
    case Format::Uint16_NormalizedRGB:
        return GL_RGB16;
    case Format::Uint16_NormalizedRGBA:
        return GL_RGBA16;
    case Format::Int16_NormalizedR:
        return GL_R16_SNORM;
    case Format::Int16_NormalizedRG:
        return GL_RG16_SNORM;
    case Format::Int16_NormalizedRGB:
        return GL_RGB16_SNORM;
    case Format::Int16_NormalizedRGBA:
        return GL_RGBA16_SNORM;
    case Format::Uint16_R:
        return GL_R16UI;
    case Format::Uint16_RG:
        return GL_RG16UI;
    case Format::Uint16_RGB:
        return GL_RGB16UI;
    case Format::Uint16_RGBA:
        return GL_RGBA16UI;
    case Format::Int16_R:
        return GL_R16I;
    case Format::Int16_RG:
        return GL_RG16I;
    case Format::Int16_RGB:
        return GL_RGB16I;
    case Format::Int16_RGBA:
        return GL_RGBA16I;
    case Format::Uint32_R:
        return GL_R32UI;
    case Format::Uint32_RG:
        return GL_RG32UI;
    case Format::Uint32_RGB:
        return GL_RGB32UI;
    case Format::Uint32_RGBA:
        return GL_RGBA32UI;
    case Format::Int32_R:
        return GL_R32I;
    case Format::Int32_RG:
        return GL_RG32I;
    case Format::Int32_RGB:
        return GL_RGB32I;
    case Format::Int32_RGBA:
        return GL_RGBA32I;
    case Format::Float16_R:
        return GL_R16F;
    case Format::Float16_RG:
        return GL_RG16F;
    case Format::Float16_RGB:
        return GL_RGB16F;
    case Format::Float16_RGBA:
        return GL_RGBA16F;
    case Format::Float32_R:
        return GL_R32F;
    case Format::Float32_RG:
        return GL_RG32F;
    case Format::Float32_RGB:
        return GL_RGB32F;
    case Format::Float32_RGBA:
        return GL_RGBA32F;
    case Format::Depth16:
        return GL_DEPTH_COMPONENT16;
    case Format::Depth24:
        return GL_DEPTH_COMPONENT24;
    case Format::Depth32:
        return GL_DEPTH_COMPONENT32;
    case Format::Depth32F:
        return GL_DEPTH_COMPONENT32F;
    case Format::Depth24_Stencil8:
        return GL_DEPTH24_STENCIL8;
    case Format::Depth32F_Stencil8:
        return GL_DEPTH32F_STENCIL8;
    case Format::Stencil8:
        return GL_STENCIL_INDEX8;
    case Format::DXT5_RGBA:
        return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
    default:
        throw std::runtime_error("Unknow Image Format");
    }
}
}
