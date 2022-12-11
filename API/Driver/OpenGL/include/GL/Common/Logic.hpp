/*
* @Author: gpinchon
* @Date:   2021-09-26 16:41:03
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 16:41:53
*/
#pragma once

#include <OCRA/Common/Logic.hpp>

#include <stdexcept>

#include <GL/glew.h>

namespace OCRA::Logic {
static inline auto GetGLOperation(const Operation& a_Operation)
{
    switch (a_Operation) {
    case Operation::Clear:
        return GL_CLEAR;
    case Operation::And:
        return GL_AND;
    case Operation::AndReverse:
        return GL_AND_REVERSE;
    case Operation::Copy:
        return GL_COPY;
    case Operation::AndInverted:
        return GL_AND_INVERTED;
    case Operation::NoOp:
        return GL_NOOP;
    case Operation::Xor:
        return GL_XOR;
    case Operation::Or:
        return GL_OR;
    case Operation::Nor:
        return GL_NOR;
    case Operation::Equivalent:
        return GL_EQUIV;
    case Operation::Invert:
        return GL_INVERT;
    case Operation::OrReverse:
        return GL_OR_REVERSE;
    case Operation::CopyInverted:
        return GL_COPY_INVERTED;
    case Operation::OrInverted:
        return GL_OR_INVERTED;
    case Operation::Nand:
        return GL_NAND;
    case Operation::Set:
        return GL_SET;
    default:
        throw std::runtime_error("Unknown Logic Operation");
    }
}
}