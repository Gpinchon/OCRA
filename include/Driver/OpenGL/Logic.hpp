/*
* @Author: gpinchon
* @Date:   2021-09-26 16:41:03
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 16:41:53
*/
#pragma once

namespace OCRA::Logic {
static inline auto GetGLLogicOperation(const Logic::Operation& a_Operation)
{
    switch (a_Operation) {
    case Logic::Operation::Clear:
        return GL_CLEAR;
    case Logic::Operation::And:
        return GL_AND;
    case Logic::Operation::AndReverse:
        return GL_AND_REVERSE;
    case Logic::Operation::Copy:
        return GL_COPY;
    case Logic::Operation::AndInverted:
        return GL_AND_INVERTED;
    case Logic::Operation::NoOp:
        return GL_NOOP;
    case Logic::Operation::Xor:
        return GL_XOR;
    case Logic::Operation::Or:
        return GL_OR;
    case Logic::Operation::Nor:
        return GL_NOR;
    case Logic::Operation::Equivalent:
        return GL_EQUIV;
    case Logic::Operation::Invert:
        return GL_INVERT;
    case Logic::Operation::OrReverse:
        return GL_OR_REVERSE;
    case Logic::Operation::CopyInverted:
        return GL_COPY_INVERTED;
    case Logic::Operation::OrInverted:
        return GL_OR_INVERTED;
    case Logic::Operation::Nand:
        return GL_NAND;
    case Logic::Operation::Set:
        return GL_SET;
    default:
        throw std::runtime_error("Unknown Logic Operation");
    }
}
}