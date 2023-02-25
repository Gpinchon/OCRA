/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-06 22:37:32
*/
#pragma once

#include <OCRA/Enums.hpp>

#include <stdexcept>

#include <GL/glew.h>

namespace OCRA {
inline auto IsCompressedFormat(const Format& format)
{
    return format == Format::S3TC_DXT5_RGBA
        || format == Format::S3TC_DXT5_SRGBA;
}
inline auto GetGLSizedFormat(const Format& format)
{
    switch (format) {
    case Format::Uint8_Normalized_R:
        return GL_R8;
    case Format::Uint8_Normalized_RG:
        return GL_RG8;
    case Format::Uint8_Normalized_RGB:
        return GL_RGB8;
    case Format::Uint8_Normalized_RGBA:
        return GL_RGBA8;
    case Format::Int8_Normalized_R:
        return GL_R8_SNORM;
    case Format::Int8_Normalized_RG:
        return GL_RG8_SNORM;
    case Format::Int8_Normalized_RGB:
        return GL_RGB8_SNORM;
    case Format::Int8_Normalized_RGBA:
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
    case Format::Uint16_Normalized_R:
        return GL_R16;
    case Format::Uint16_Normalized_RG:
        return GL_RG16;
    case Format::Uint16_Normalized_RGB:
        return GL_RGB16;
    case Format::Uint16_Normalized_RGBA:
        return GL_RGBA16;
    case Format::Int16_Normalized_R:
        return GL_R16_SNORM;
    case Format::Int16_Normalized_RG:
        return GL_RG16_SNORM;
    case Format::Int16_Normalized_RGB:
        return GL_RGB16_SNORM;
    case Format::Int16_Normalized_RGBA:
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
    case Format::Uint16_Normalized_Depth:
        return GL_DEPTH_COMPONENT16;
    case Format::Float32_Depth:
        return GL_DEPTH_COMPONENT32F;
    case Format::Uint24_Normalized_Depth_Uint8_Stencil:
        return GL_DEPTH24_STENCIL8;
    case Format::Float32_Normalized_Depth_Uint8_Stencil:
        return GL_DEPTH32F_STENCIL8;
    case Format::Uint8_Stencil:
        return GL_STENCIL_INDEX8;
    case Format::S3TC_DXT5_RGBA:
        return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
    case Format::S3TC_DXT5_SRGBA:
        return GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT;
    default:
        throw std::runtime_error("Unknow Image Format");
    }
}
inline auto GetGLDataFormat(const Format& format)
{
    switch (format) {
    case Format::Uint8_Normalized_R:
    case Format::Int8_Normalized_R:
    case Format::Uint16_Normalized_R:
    case Format::Int16_Normalized_R:
    case Format::Float16_R:
    case Format::Float32_R:
        return GL_RED;
    case Format::Uint16_R:
    case Format::Int16_R:
    case Format::Uint32_R:
    case Format::Int32_R:
    case Format::Uint8_R:
    case Format::Int8_R:
        return GL_RED_INTEGER;
    case Format::Uint8_Normalized_RG:
    case Format::Int8_Normalized_RG:
    case Format::Uint16_Normalized_RG:
    case Format::Int16_Normalized_RG:
    case Format::Float16_RG:
    case Format::Float32_RG:
        return GL_RG;
    case Format::Uint8_RG:
    case Format::Int8_RG:
    case Format::Uint16_RG:
    case Format::Int16_RG:
    case Format::Uint32_RG:
    case Format::Int32_RG:
        return GL_RG_INTEGER;
    case Format::Uint8_Normalized_RGB:
    case Format::Int8_Normalized_RGB:
    case Format::Uint16_Normalized_RGB:
    case Format::Int16_Normalized_RGB:
    case Format::Float16_RGB:
    case Format::Float32_RGB:
        return GL_RGB;
    case Format::Uint8_RGB:
    case Format::Int8_RGB:
    case Format::Uint16_RGB:
    case Format::Int16_RGB:
    case Format::Uint32_RGB:
    case Format::Int32_RGB:
        return GL_RGB_INTEGER;
    case Format::Uint8_Normalized_RGBA:
    case Format::Int8_Normalized_RGBA:
    case Format::Uint16_Normalized_RGBA:
    case Format::Int16_Normalized_RGBA:
    case Format::Float16_RGBA:
    case Format::Float32_RGBA:
        return GL_RGBA;
    case Format::S3TC_DXT5_RGBA:
    case Format::S3TC_DXT5_SRGBA:
        return GL_COMPRESSED_RGBA;
    case Format::Uint8_RGBA:
    case Format::Int8_RGBA:
    case Format::Uint16_RGBA:
    case Format::Int16_RGBA:
    case Format::Uint32_RGBA:
    case Format::Int32_RGBA:
        return GL_RGBA_INTEGER;
    case Format::Uint16_Normalized_Depth:
    case Format::Float32_Depth:
        return GL_DEPTH_COMPONENT;
    case Format::Uint24_Normalized_Depth_Uint8_Stencil:
    case Format::Float32_Normalized_Depth_Uint8_Stencil:
        return GL_DEPTH_STENCIL;
    case Format::Uint8_Stencil:
        return GL_STENCIL_INDEX;
    default:
        throw std::runtime_error("Unknow Image Format");
    }
}
inline auto GetGLClearColorType(const Format& a_Format)
{
    switch (a_Format) {
    case Format::Uint8_Normalized_R:
    case Format::Uint8_Normalized_RG:
    case Format::Uint8_Normalized_RGB:
    case Format::Uint8_Normalized_RGBA:
    case Format::Int8_Normalized_R:
    case Format::Int8_Normalized_RG:
    case Format::Int8_Normalized_RGB:
    case Format::Int8_Normalized_RGBA:
    case Format::Uint16_Normalized_R:
    case Format::Uint16_Normalized_RG:
    case Format::Uint16_Normalized_RGB:
    case Format::Uint16_Normalized_RGBA:
    case Format::Uint16_Normalized_Depth:
    case Format::Int16_Normalized_R:
    case Format::Int16_Normalized_RG:
    case Format::Int16_Normalized_RGB:
    case Format::Int16_Normalized_RGBA:
    case Format::Float16_R:
    case Format::Float16_RG:
    case Format::Float16_RGB:
    case Format::Float16_RGBA:
    case Format::Float32_R:
    case Format::Float32_RG:
    case Format::Float32_RGB:
    case Format::Float32_RGBA:
    case Format::Float32_Depth:
        return GL_FLOAT;
    case Format::Uint8_R:
    case Format::Uint8_RG:
    case Format::Uint8_RGB:
    case Format::Uint8_RGBA:
    case Format::Uint16_R:
    case Format::Uint16_RG:
    case Format::Uint16_RGB:
    case Format::Uint16_RGBA:
    case Format::Uint32_R:
    case Format::Uint32_RG:
    case Format::Uint32_RGB:
    case Format::Uint32_RGBA:
        return GL_UNSIGNED_INT;
    case Format::Int8_R:
    case Format::Int8_RG:
    case Format::Int8_RGB:
    case Format::Int8_RGBA:
    case Format::Int16_R:
    case Format::Int16_RG:
    case Format::Int16_RGB:
    case Format::Int16_RGBA:
    case Format::Int32_R:
    case Format::Int32_RG:
    case Format::Int32_RGB:
    case Format::Int32_RGBA:
        return GL_INT;
    default:
        throw std::runtime_error("Incorrect Image Format");
    }
}
inline auto GetGLDataType(const Format& a_Format)
{
    switch (a_Format) {
    case Format::Uint8_Normalized_R:
    case Format::Uint8_R:
    case Format::Uint8_Normalized_RG:
    case Format::Uint8_RG:
    case Format::Uint8_Normalized_RGB:
    case Format::Uint8_RGB:
    case Format::Uint8_Normalized_RGBA:
    case Format::Uint8_RGBA:
        return GL_UNSIGNED_BYTE;
    case Format::Int8_Normalized_R:
    case Format::Int8_R:
    case Format::Int8_Normalized_RG:
    case Format::Int8_RG:
    case Format::Int8_Normalized_RGB:
    case Format::Int8_RGB:
    case Format::Int8_Normalized_RGBA:
    case Format::Int8_RGBA:
        return GL_BYTE;
    case Format::Uint16_Normalized_R:
    case Format::Uint16_R:
    case Format::Uint16_Normalized_RG:
    case Format::Uint16_RG:
    case Format::Uint16_Normalized_RGB:
    case Format::Uint16_RGB:
    case Format::Uint16_Normalized_RGBA:
    case Format::Uint16_RGBA:
    case Format::Uint16_Normalized_Depth:
        return GL_UNSIGNED_SHORT;
    case Format::Int16_Normalized_R:
    case Format::Int16_R:
    case Format::Int16_Normalized_RG:
    case Format::Int16_RG:
    case Format::Int16_Normalized_RGB:
    case Format::Int16_RGB:
    case Format::Int16_Normalized_RGBA:
    case Format::Int16_RGBA:
        return GL_SHORT;
    case Format::Uint32_R:
    case Format::Uint32_RG:
    case Format::Uint32_RGB:
    case Format::Uint32_RGBA:
        return GL_UNSIGNED_INT;
    case Format::Int32_R:
    case Format::Int32_RG:
    case Format::Int32_RGB:
    case Format::Int32_RGBA:
        return GL_INT;
    case Format::Float16_R:
    case Format::Float16_RG:
    case Format::Float16_RGB:
    case Format::Float16_RGBA:
        return GL_HALF_FLOAT;
    case Format::Float32_R:
    case Format::Float32_RG:
    case Format::Float32_RGB:
    case Format::Float32_RGBA:
    case Format::Float32_Depth:
        return GL_FLOAT;
    case Format::Uint24_Normalized_Depth_Uint8_Stencil:
        return GL_UNSIGNED_INT_24_8;
    case Format::Float32_Normalized_Depth_Uint8_Stencil:
        return GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
    case Format::Uint8_Stencil:
        return GL_UNSIGNED_BYTE;
    case Format::S3TC_DXT5_RGBA:
        return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
    case Format::S3TC_DXT5_SRGBA:
        return GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT;
    default:
        throw std::runtime_error("Unknow Image Format");
    }
}
inline auto GetRedSize(const Format& format)
{
    switch (format) {
    case Format::Uint8_Normalized_R:
    case Format::Uint8_R:
    case Format::Uint8_Normalized_RG:
    case Format::Uint8_RG:
    case Format::Uint8_Normalized_RGB:
    case Format::Uint8_RGB:
    case Format::Uint8_Normalized_RGBA:
    case Format::Uint8_RGBA:
    case Format::Int8_Normalized_R:
    case Format::Int8_R:
    case Format::Int8_Normalized_RG:
    case Format::Int8_RG:
    case Format::Int8_Normalized_RGB:
    case Format::Int8_RGB:
    case Format::Int8_Normalized_RGBA:
    case Format::Int8_RGBA:
        return 8;
    case Format::Uint16_Normalized_R:
    case Format::Uint16_R:
    case Format::Uint16_Normalized_RG:
    case Format::Uint16_RG:
    case Format::Uint16_Normalized_RGB:
    case Format::Uint16_RGB:
    case Format::Uint16_Normalized_RGBA:
    case Format::Uint16_RGBA:
    case Format::Int16_Normalized_R:
    case Format::Int16_R:
    case Format::Int16_Normalized_RG:
    case Format::Int16_RG:
    case Format::Int16_Normalized_RGB:
    case Format::Int16_RGB:
    case Format::Int16_Normalized_RGBA:
    case Format::Int16_RGBA:
    case Format::Float16_R:
    case Format::Float16_RG:
    case Format::Float16_RGB:
    case Format::Float16_RGBA:
        return 16;
    case Format::Uint32_R:
    case Format::Uint32_RG:
    case Format::Uint32_RGB:
    case Format::Uint32_RGBA:
    case Format::Int32_R:
    case Format::Int32_RG:
    case Format::Int32_RGB:
    case Format::Int32_RGBA:
    case Format::Float32_R:
    case Format::Float32_RG:
    case Format::Float32_RGB:
    case Format::Float32_RGBA:
        return 32;
    default:
        return 0;
    }
}
inline auto GetGreenSize(const Format& format)
{
    switch (format) {
    case Format::Uint8_Normalized_RG:
    case Format::Uint8_RG:
    case Format::Uint8_Normalized_RGB:
    case Format::Uint8_RGB:
    case Format::Uint8_Normalized_RGBA:
    case Format::Uint8_RGBA:
    case Format::Int8_Normalized_RG:
    case Format::Int8_RG:
    case Format::Int8_Normalized_RGB:
    case Format::Int8_RGB:
    case Format::Int8_Normalized_RGBA:
    case Format::Int8_RGBA:
        return 8;
    case Format::Uint16_Normalized_RG:
    case Format::Uint16_RG:
    case Format::Uint16_Normalized_RGB:
    case Format::Uint16_RGB:
    case Format::Uint16_Normalized_RGBA:
    case Format::Uint16_RGBA:
    case Format::Int16_Normalized_RG:
    case Format::Int16_RG:
    case Format::Int16_Normalized_RGB:
    case Format::Int16_RGB:
    case Format::Int16_Normalized_RGBA:
    case Format::Int16_RGBA:
    case Format::Float16_RG:
    case Format::Float16_RGB:
    case Format::Float16_RGBA:
        return 16;
    case Format::Uint32_RG:
    case Format::Uint32_RGB:
    case Format::Uint32_RGBA:
    case Format::Int32_RG:
    case Format::Int32_RGB:
    case Format::Int32_RGBA:
    case Format::Float32_RG:
    case Format::Float32_RGB:
    case Format::Float32_RGBA:
        return 32;
    default:
        return 0;
    }
}
inline auto GetBlueSize(const Format& format)
{
    switch (format) {
    case Format::Uint8_Normalized_RGB:
    case Format::Uint8_RGB:
    case Format::Uint8_Normalized_RGBA:
    case Format::Uint8_RGBA:
    case Format::Int8_Normalized_RGB:
    case Format::Int8_RGB:
    case Format::Int8_Normalized_RGBA:
    case Format::Int8_RGBA:
        return 8;
    case Format::Uint16_Normalized_RGB:
    case Format::Uint16_RGB:
    case Format::Uint16_Normalized_RGBA:
    case Format::Uint16_RGBA:
    case Format::Int16_Normalized_RGB:
    case Format::Int16_RGB:
    case Format::Int16_Normalized_RGBA:
    case Format::Int16_RGBA:
    case Format::Float16_RGB:
    case Format::Float16_RGBA:
        return 16;
    case Format::Uint32_RGB:
    case Format::Uint32_RGBA:
    case Format::Int32_RGB:
    case Format::Int32_RGBA:
    case Format::Float32_RGB:
    case Format::Float32_RGBA:
        return 32;
    default:
        return 0;
    }
}
inline auto GetAlphaSize(const Format& format)
{
    switch (format) {
    case Format::Uint8_Normalized_RGBA:
    case Format::Uint8_RGBA:
    case Format::Int8_Normalized_RGBA:
    case Format::Int8_RGBA:
        return 8;
    case Format::Uint16_Normalized_RGBA:
    case Format::Uint16_RGBA:
    case Format::Int16_Normalized_RGBA:
    case Format::Int16_RGBA:
    case Format::Float16_RGBA:
        return 16;
    case Format::Uint32_RGBA:
    case Format::Int32_RGBA:
    case Format::Float32_RGBA:
        return 32;
    default:
        return 0;
    }
}
inline auto GetDepthSize(const Format& format)
{
    switch (format) {
    case Format::Uint16_Normalized_Depth:
        return 16;
    case Format::Uint24_Normalized_Depth_Uint8_Stencil:
        return 24;
    case Format::Float32_Depth:
    case Format::Float32_Normalized_Depth_Uint8_Stencil:
        return 32;
    default:
        return 0;
    }
}
inline auto GetStencilSize(const Format& format)
{
    switch (format) {
    case Format::Uint24_Normalized_Depth_Uint8_Stencil:
    case Format::Float32_Normalized_Depth_Uint8_Stencil:
    case Format::Uint8_Stencil:
        return 8;
    default:
        return 0;
    }
}
//inline auto GetPixelSize(const Format& a_Format)
//{
//    return
//        GetRedSize(a_Format) + GetGreenSize(a_Format) +
//        GetRedSize(a_Format) + GetAlphaSize(a_Format) +
//        GetDepthSize(a_Format) + GetStencilSize(a_Format);
//}
}
