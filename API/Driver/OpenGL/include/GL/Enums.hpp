#pragma once

#include <OCRA/Enums.hpp>

#include <GL/glew.h>

#include <stdexcept>

namespace OCRA
{
static inline auto GetGLBlendOp(const BlendOp& a_Operation)
{
    switch (a_Operation) {
    case BlendOp::Add:
        return GL_FUNC_ADD;
    case BlendOp::Substract:
        return GL_FUNC_SUBTRACT;
    case BlendOp::ReverseSubstract:
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
        if (a_SwizzleComponent == GL_TEXTURE_SWIZZLE_R) return GL_RED;
        else if (a_SwizzleComponent == GL_TEXTURE_SWIZZLE_G) return GL_GREEN;
        else if (a_SwizzleComponent == GL_TEXTURE_SWIZZLE_B) return GL_BLUE;
        else if (a_SwizzleComponent == GL_TEXTURE_SWIZZLE_A) return GL_ALPHA;
        break;
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
}