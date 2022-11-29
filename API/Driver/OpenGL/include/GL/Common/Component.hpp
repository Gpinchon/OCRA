/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:28:28
*/
#pragma once

#include <Common/Component.hpp>

#include <GL/glew.h>

namespace OCRA::Component {
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
struct GLSwizzleMask
{
    GLSwizzleMask(const Mapping& a_Mapping)
        : r(GetGLSwizzle(a_Mapping.r, GL_TEXTURE_SWIZZLE_R))
        , g(GetGLSwizzle(a_Mapping.g, GL_TEXTURE_SWIZZLE_G))
        , b(GetGLSwizzle(a_Mapping.b, GL_TEXTURE_SWIZZLE_B))
        , a(GetGLSwizzle(a_Mapping.a, GL_TEXTURE_SWIZZLE_A))
    {}
    const GLenum r;
    const GLenum g;
    const GLenum b;
    const GLenum a;
};
}