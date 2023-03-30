#pragma once

#include <OCRA/Enums.hpp>

#include <GL/glew.h>

#include <stdexcept>

namespace OCRA
{
static inline auto GetGLImageViewType(const ImageViewType& a_Type, const bool& a_Multisample)
{
    switch (a_Type)
    {
    case ImageViewType::View1D:
        return a_Multisample ? GL_NONE : GL_TEXTURE_1D;
    case ImageViewType::View2D:
        return a_Multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    case ImageViewType::View3D:
        return a_Multisample ? GL_NONE : GL_TEXTURE_3D;
    case ImageViewType::ViewCube:
        return a_Multisample ? GL_NONE : GL_TEXTURE_CUBE_MAP;
    case ImageViewType::View1DArray:
        return a_Multisample ? GL_NONE : GL_TEXTURE_1D_ARRAY;
    case ImageViewType::View2DArray:
        return a_Multisample ? GL_TEXTURE_2D_MULTISAMPLE_ARRAY : GL_TEXTURE_2D_ARRAY;
    case ImageViewType::ViewCubeArray:
        return a_Multisample ? GL_NONE : GL_TEXTURE_CUBE_MAP_ARRAY;
    default:
        throw std::runtime_error("Unknown Image View Type");
    }
}
static inline auto GetGLBlendOp(const BlendOp& a_Operation)
{
    switch (a_Operation) {
    case BlendOp::Add:
        return GL_FUNC_ADD;
    case BlendOp::Subtract:
        return GL_FUNC_SUBTRACT;
    case BlendOp::ReverseSubtract:
        return GL_FUNC_REVERSE_SUBTRACT;
    case BlendOp::Min:
        return GL_MIN;
    case BlendOp::Max:
        return GL_MAX;
    default:
        throw std::runtime_error("Unknown Blend Operation");
    }
}
static inline auto GetGLBlendFactor(const BlendFactor& a_Factor)
{
    switch (a_Factor) {
    case BlendFactor::Zero:
        return GL_ZERO;
    case BlendFactor::One:
        return GL_ONE;
    case BlendFactor::SrcColor:
        return GL_SRC_COLOR;
    case BlendFactor::OneMinusSrcColor:
        return GL_ONE_MINUS_SRC_COLOR;
    case BlendFactor::DstColor:
        return GL_DST_COLOR;
    case BlendFactor::OneMinusDstColor:
        return GL_ONE_MINUS_DST_COLOR;
    case BlendFactor::SrcAlpha:
        return GL_SRC_ALPHA;
    case BlendFactor::OneMinusSrcAlpha:
        return GL_ONE_MINUS_SRC_ALPHA;
    case BlendFactor::DstAlpha:
        return GL_DST_ALPHA;
    case BlendFactor::OneMinusDstAlpha:
        return GL_ONE_MINUS_DST_ALPHA;
    case BlendFactor::ConstantColor:
        return GL_CONSTANT_COLOR;
    case BlendFactor::OneMinusConstantColor:
        return GL_ONE_MINUS_CONSTANT_COLOR;
    case BlendFactor::ConstantAlpha:
        return GL_CONSTANT_ALPHA;
    case BlendFactor::OneMinusConstantAlpha:
        return GL_ONE_MINUS_CONSTANT_ALPHA;
    case BlendFactor::SrcAlphaSaturate:
        return GL_SRC_ALPHA_SATURATE;
    case BlendFactor::Src1Color:
        return GL_SRC1_COLOR;
    case BlendFactor::OneMinusSrc1Color:
        return GL_ONE_MINUS_SRC1_COLOR;
    case BlendFactor::Src1Alpha:
        return GL_SRC1_ALPHA;
    case BlendFactor::OneMinusSrc1Alpha:
        return GL_ONE_MINUS_SRC1_ALPHA;
    default:
        throw std::runtime_error("Unknown Blend Factor");
    }
}
static inline auto GetGLOperation(const CompareOp& a_Operation)
{
    switch (a_Operation)
    {
    case CompareOp::Never:
        return GL_NEVER;
    case CompareOp::Less:
        return GL_LESS;
    case CompareOp::Equal:
        return GL_EQUAL;
    case CompareOp::LessOrEqual:
        return GL_LEQUAL;
    case CompareOp::Greater:
        return GL_GREATER;
    case CompareOp::NotEqual:
        return GL_NOTEQUAL;
    case CompareOp::GreaterOrEqual:
        return GL_GEQUAL;
    case CompareOp::Always:
        return GL_ALWAYS;
    default:
        throw std::runtime_error("Unknown Compare Operation");
    }
}
inline GLenum GetGLSwizzle(const Swizzle& a_Swizzle, const GLenum& a_SwizzleComponent) {
    switch (a_Swizzle)
    {
    case Swizzle::Identity:
        return a_SwizzleComponent;
    case Swizzle::Zero:
        return GL_ZERO;
    case Swizzle::One:
        return GL_ONE;
    case Swizzle::R:
        return GL_RED;
    case Swizzle::G:
        return GL_GREEN;
    case Swizzle::B:
        return GL_BLUE;
    case Swizzle::A:
        return GL_ALPHA;
    default:
        break;
    }
    throw std::runtime_error("Unknown Swizzle Type");
    return 0;
}
static inline auto GetGLIndexType(const IndexType& a_IndexType)
{
    switch (a_IndexType)
    {
    case IndexType::None:
        return GL_NONE;
    case IndexType::Uint32:
        return GL_UNSIGNED_INT;
    case IndexType::Uint16:
        return GL_UNSIGNED_SHORT;
    case IndexType::Uint8:
        return GL_UNSIGNED_BYTE;
    default:
        throw std::runtime_error("Unknown Index Type");
    }
}
static inline auto GetIndexTypeSize(const GLenum& a_IndexType)
{
    switch (a_IndexType) {
    case GL_UNSIGNED_BYTE :
        return 1;
    case GL_UNSIGNED_SHORT :
        return 2;
    case GL_UNSIGNED_INT :
        return 4;
    default : throw std::runtime_error("Unknown Index Type");
    }
}
static inline auto GetGLLogicOp(const LogicOp& a_LogicOp)
{
    switch (a_LogicOp) {
    case LogicOp::Clear:
        return GL_CLEAR;
    case LogicOp::And:
        return GL_AND;
    case LogicOp::AndReverse:
        return GL_AND_REVERSE;
    case LogicOp::Copy:
        return GL_COPY;
    case LogicOp::AndInverted:
        return GL_AND_INVERTED;
    case LogicOp::NoOp:
        return GL_NOOP;
    case LogicOp::Xor:
        return GL_XOR;
    case LogicOp::Or:
        return GL_OR;
    case LogicOp::Nor:
        return GL_NOR;
    case LogicOp::Equivalent:
        return GL_EQUIV;
    case LogicOp::Invert:
        return GL_INVERT;
    case LogicOp::OrReverse:
        return GL_OR_REVERSE;
    case LogicOp::CopyInverted:
        return GL_COPY_INVERTED;
    case LogicOp::OrInverted:
        return GL_OR_INVERTED;
    case LogicOp::Nand:
        return GL_NAND;
    case LogicOp::Set:
        return GL_SET;
    default:
        throw std::runtime_error("Unknown Logic Operation");
    }
}
static inline auto GetGLPolygonMode(const PolygonMode& a_Mode)
{
    switch (a_Mode)
    {
    case PolygonMode::Fill :
        return GL_FILL;
    case PolygonMode::Line :
        return GL_LINE;
    case PolygonMode::Point :
        return GL_POINT;
    default:
        throw std::runtime_error("Unknown Polygon Mode");
    }
}
static inline auto GetGLOperation(const StencilOp& a_Operation) {
    switch (a_Operation)
    {
    case StencilOp::Keep:
        return GL_KEEP;
    case StencilOp::Zero:
        return GL_ZERO;
    case StencilOp::Replace:
        return GL_REPLACE;
    case StencilOp::IncrementAndClamp:
        return GL_INCR;
    case StencilOp::DecrementAndClamp:
        return GL_DECR;
    case StencilOp::Invert:
        return GL_INVERT;
    case StencilOp::IncrementAndWrap:
        return GL_INCR_WRAP;
    case StencilOp::DecrementAndWrap:
        return GL_DECR_WRAP;
    default:
        throw std::runtime_error("Unknown Stencil Operation");
    }
}
static inline auto GetGLVertexType(const VertexType& a_VertexType)
{
    switch (a_VertexType)
    {
    case OCRA::VertexType::None:
        return GL_NONE;
    case OCRA::VertexType::Float32:
        return GL_FLOAT;
    case OCRA::VertexType::Float16:
        return GL_HALF_FLOAT;
    case OCRA::VertexType::Int32:
        return GL_INT;
    case OCRA::VertexType::Uint32:
        return GL_UNSIGNED_INT;
    case OCRA::VertexType::Int16:
        return GL_SHORT;
    case OCRA::VertexType::Uint16:
        return GL_UNSIGNED_SHORT;
    case OCRA::VertexType::Int8:
        return GL_BYTE;
    case OCRA::VertexType::Uint8:
        return GL_UNSIGNED_BYTE;
    default:
        throw std::runtime_error("Unknown Vertex Type");
    }
}
static inline auto GetGLPolygonOffsetMode(const PolygonMode& a_Mode)
{
    switch (a_Mode)
    {
    case PolygonMode::Fill :
        return GL_POLYGON_OFFSET_FILL;
    case PolygonMode::Line :
        return GL_POLYGON_OFFSET_LINE;
    case PolygonMode::Point :
        return GL_POLYGON_OFFSET_POINT;
    default:
        throw std::runtime_error("Unknown Polygon Mode");
    }
}
static inline auto GetGLCullMode(const CullMode& a_Mode)
{
    switch (a_Mode)
    {
    case CullMode::None :
        return GL_NONE;
    case CullMode::Front :
        return GL_FRONT;
    case CullMode::Back :
        return GL_BACK;
    case CullMode::FrontAndBack :
        return GL_FRONT_AND_BACK;
    default:
        throw std::runtime_error("Unknown Cull Mode");
    }
}
static inline auto GetGLFrontFace(const FrontFace& a_FrontFace)
{
    switch (a_FrontFace)
    {
    case FrontFace::Clockwise :
        return GL_CW;
    case FrontFace::CounterClockwise:
        return GL_CCW;
    default :
        throw std::runtime_error("Unknown Front Face");
    }
}

static inline auto GetGLAddressMode(const SamplerAddressMode& a_AddressMode) {
    switch (a_AddressMode)
    {
    case SamplerAddressMode::Repeat:
        return GL_REPEAT;
    case SamplerAddressMode::MirroredRepeat:
        return GL_MIRRORED_REPEAT;
    case SamplerAddressMode::ClampToEdge:
        return GL_CLAMP_TO_EDGE;
    case SamplerAddressMode::ClampToBorder:
        return GL_CLAMP_TO_BORDER;
    default:
        throw std::runtime_error("Unknown Address Mode");
    }
}

static inline auto GetGLMagFilter(const Filter& a_Filter) {
    switch (a_Filter)
    {
    case Filter::Nearest:
        return GL_NEAREST;
    case Filter::Linear:
        return GL_LINEAR;
    default:
        throw std::runtime_error("Unknown Filter Mode");
    }
}

static inline auto GetGLMinFilter(const Filter& a_Filter, const Filter& a_MipmapMode) {
    switch (a_Filter)
    {
    case Filter::Nearest:
        return (a_MipmapMode == Filter::Nearest) ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST_MIPMAP_LINEAR;
    case Filter::Linear:
        return (a_MipmapMode == Filter::Nearest) ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR_MIPMAP_LINEAR;;
    default:
        throw std::runtime_error("Unknown Filter Mode");
    }
}
static inline auto IsCompressedFormat(const Format& format)
{
    return format == Format::S3TC_DXT5_RGBA
        || format == Format::S3TC_DXT5_SRGBA;
}
static inline auto GetGLSizedFormat(const Format& format)
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
    case Format::Uint8_Normalized_BGR:
        return GL_BGR;
    case Format::Uint8_Normalized_BGRA:
        return GL_BGRA8_EXT;
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
static inline auto GetGLDataFormat(const Format& format)
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
    case Format::Uint8_Normalized_BGR:
        return GL_BGR;
    case Format::Uint8_Normalized_BGRA:
        return GL_BGRA;
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
static inline auto GetGLClearColorType(const Format& a_Format)
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
static inline auto GetGLDataType(const Format& a_Format)
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
    case Format::Uint8_Normalized_BGR:
    case Format::Uint8_Normalized_BGRA:
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
static inline auto GetRedSize(const Format& format)
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
static inline auto GetGreenSize(const Format& format)
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
static inline auto GetBlueSize(const Format& format)
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
static inline auto GetAlphaSize(const Format& format)
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
static inline auto GetDepthSize(const Format& format)
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
static inline auto GetStencilSize(const Format& format)
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
static inline auto GetPixelSize(const Format& a_Format)
{
    return
        GetRedSize(a_Format) + GetGreenSize(a_Format) +
        GetBlueSize(a_Format) + GetAlphaSize(a_Format) +
        GetDepthSize(a_Format) + GetStencilSize(a_Format);
}
}