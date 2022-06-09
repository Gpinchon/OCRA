/*
* @Author: gpinchon
* @Date:   2021-09-26 16:43:43
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-27 19:48:31
*/
#pragma once

#include <Common/Blend.hpp>

#include <GL/glew.h>

#include <stdexcept>

namespace OCRA::Blend {
static inline auto GetGLOperation(const Operation& a_Operation)
{
    switch (a_Operation) {
    case Operation::Add:
        return GL_FUNC_ADD;
    case Operation::Substract:
        return GL_FUNC_SUBTRACT;
    case Operation::ReverseSubstract:
        return GL_FUNC_REVERSE_SUBTRACT;
    case Operation::Min:
        return GL_MIN;
    case Operation::Max:
        return GL_MAX;
    default:
        throw std::runtime_error("Unknown Blend Operation");
    }
}
static inline auto GetGLFactor(const Factor& a_Factor)
{
    switch (a_Factor) {
    case Factor::Zero:
        return GL_ZERO;
    case Factor::One:
        return GL_ONE;
    case Factor::SrcColor:
        return GL_SRC_COLOR;
    case Factor::OneMinusSrcColor:
        return GL_ONE_MINUS_SRC_COLOR;
    case Factor::DstColor:
        return GL_DST_COLOR;
    case Factor::OneMinusDstColor:
        return GL_ONE_MINUS_DST_COLOR;
    case Factor::SrcAlpha:
        return GL_SRC_ALPHA;
    case Factor::OneMinusSrcAlpha:
        return GL_ONE_MINUS_SRC_ALPHA;
    case Factor::DstAlpha:
        return GL_DST_ALPHA;
    case Factor::OneMinusDstAlpha:
        return GL_ONE_MINUS_DST_ALPHA;
    case Factor::ConstantColor:
        return GL_CONSTANT_COLOR;
    case Factor::OneMinusConstantColor:
        return GL_ONE_MINUS_CONSTANT_COLOR;
    case Factor::ConstantAlpha:
        return GL_CONSTANT_ALPHA;
    case Factor::OneMinusConstantAlpha:
        return GL_ONE_MINUS_CONSTANT_ALPHA;
    case Factor::SrcAlphaSaturate:
        return GL_SRC_ALPHA_SATURATE;
    case Factor::Src1Color:
        return GL_SRC1_COLOR;
    case Factor::OneMinusSrc1Color:
        return GL_ONE_MINUS_SRC1_COLOR;
    case Factor::Src1Alpha:
        return GL_SRC1_ALPHA;
    case Factor::OneMinusSrc1Alpha:
        return GL_ONE_MINUS_SRC1_ALPHA;
    default:
        throw std::runtime_error("Unknown Blend Factor");
    }
}
}