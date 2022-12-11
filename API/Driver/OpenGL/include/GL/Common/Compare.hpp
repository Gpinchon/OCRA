/*
* @Author: gpinchon
* @Date:   2021-09-27 19:13:51
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-27 19:14:06
*/
#pragma once

#include <OCRA/Common/Compare.hpp>

#include <GL/glew.h>

#include <stdexcept>

namespace OCRA::Compare {
static inline auto GetGLOperation(const Operation& a_Operation)
{
    switch (a_Operation)
    {
    case Operation::Never:
        return GL_NEVER;
    case Operation::Less:
        return GL_LESS;
    case Operation::Equal:
        return GL_EQUAL;
    case Operation::LessOrEqual:
        return GL_LEQUAL;
    case Operation::Greater:
        return GL_GREATER;
    case Operation::NotEqual:
        return GL_NOTEQUAL;
    case Operation::GreaterOrEqual:
        return GL_GEQUAL;
    case Operation::Always:
        return GL_ALWAYS;
    default:
        throw std::runtime_error("Unknown Compare Operation");
    }
}
}