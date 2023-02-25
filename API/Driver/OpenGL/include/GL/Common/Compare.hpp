/*
* @Author: gpinchon
* @Date:   2021-09-27 19:13:51
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-27 19:14:06
*/
#pragma once

#include <OCRA/Enums.hpp>

#include <GL/glew.h>

#include <stdexcept>

namespace OCRA {
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
}