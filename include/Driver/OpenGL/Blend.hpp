/*
* @Author: gpinchon
* @Date:   2021-09-26 16:43:43
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 16:44:21
*/
#pragma once

#include <GL/glew.h>

#include <stdexcept>

namespace OCRA::Blend {
static inline auto GetGLBlendOperation(const Blend::Operation& a_Operation)
{
    switch (a_Operation) {
    case Blend::Operation::Add:
        return GL_FUNC_ADD;
    case Blend::Operation::Substract:
        return GL_FUNC_SUBTRACT;
    case Blend::Operation::ReverseSubstract:
        return GL_FUNC_REVERSE_SUBTRACT;
    case Blend::Operation::Min:
        return GL_MIN;
    case Blend::Operation::Max:
        return GL_MAX;
    default:
        throw std::runtime_error("Unknown Blend Operation");
    }
}
static inline auto GetGLBlendFactor(const Blend::Factor& a_Factor)
{
    switch (a_Factor) {
    case Blend::Factor::Zero:
        return GL_ZERO;
    case Blend::Factor::One:
        return GL_ONE;
    case Blend::Factor::SrcColor:
        return GL_SRC_COLOR;
    case Blend::Factor::OneMinusSrcColor:
        return GL_ONE_MINUS_SRC_COLOR;
    case Blend::Factor::DstColor:
        return GL_DST_COLOR;
    case Blend::Factor::OneMinusDstColor:
        return GL_ONE_MINUS_DST_COLOR;
    case Blend::Factor::SrcAlpha:
        return GL_SRC_ALPHA;
    case Blend::Factor::OneMinusSrcAlpha:
        return GL_ONE_MINUS_SRC_ALPHA;
    case Blend::Factor::DstAlpha:
        return GL_DST_ALPHA;
    case Blend::Factor::OneMinusDstAlpha:
        return GL_ONE_MINUS_DST_ALPHA;
    case Blend::Factor::ConstantColor:
        return GL_CONSTANT_COLOR;
    case Blend::Factor::OneMinusConstantColor:
        return GL_ONE_MINUS_CONSTANT_COLOR;
    case Blend::Factor::ConstantAlpha:
        return GL_CONSTANT_ALPHA;
    case Blend::Factor::OneMinusConstantAlpha:
        return GL_ONE_MINUS_CONSTANT_ALPHA;
    case Blend::Factor::SrcAlphaSaturate:
        return GL_SRC_ALPHA_SATURATE;
    case Blend::Factor::Src1Color:
        return GL_SRC1_COLOR;
    case Blend::Factor::OneMinusSrc1Color:
        return GL_ONE_MINUS_SRC1_COLOR;
    case Blend::Factor::Src1Alpha:
        return GL_SRC1_ALPHA;
    case Blend::Factor::OneMinusSrc1Alpha:
        return GL_ONE_MINUS_SRC1_ALPHA;
    default:
        throw std::runtime_error("Unknown Blend Factor");
    }
}
}