/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:31:50
*/
#pragma once

#include <Handle.hpp>
#include <Image/View.hpp>

#include <stdexcept>

#include <GL/Image/Format.hpp>
#include <GL/Image/Image.hpp>
#include <GL/Common/Component.hpp>
#include <GL/glew.h>

namespace OCRA::Image::View {
GLenum GetGLType(const Type& a_Type)
{
	switch (a_Type)
	{
	case Type::View1D:
		return GL_TEXTURE_1D;
	case Type::View2D:
		return GL_TEXTURE_2D;
	case Type::View3D:
		return GL_TEXTURE_3D;
	case Type::ViewCube:
		return GL_TEXTURE_CUBE_MAP;
	case Type::View1DArray:
		return GL_TEXTURE_1D_ARRAY;
	case Type::View2DArray:
		return GL_TEXTURE_2D_ARRAY;
	case Type::ViewCubeArray:
		return GL_TEXTURE_CUBE_MAP_ARRAY;
	default:
		throw std::runtime_error("Unknown Image View Type");
	}
}
struct Impl {
	Impl(const Device::Handle& a_Device, const Info& a_Info)
	: info(a_Info)
	{
		glGenTextures(1, &handle);
		glTextureView(
			handle,
			GetGLType(a_Info.type),
			Image::GetGLHandle(a_Device, a_Info.image),
			Image::GetGLSizedFormat(a_Info.format),
			a_Info.subRange.baseMipLevel,
			a_Info.subRange.levelCount,
			a_Info.subRange.baseArrayLayer,
			a_Info.subRange.layerCount);
		glTextureParameterfv(
			handle,
			GL_TEXTURE_SWIZZLE_RGBA,
			(float*)&Component::GLSwizzleMask(a_Info.components)
		);
	}
    ~Impl()
    {
        glDeleteTextures(1, &handle);
    }
    GLuint handle { 0 };
    const Info info;
};
Handle Create(const Device::Handle& a_Device, const Info& a_Info)
{
    return Handle(new Impl(a_Device, a_Info));
}
const Info& GetInfo(const Device::Handle& a_Device, const Handle& a_Handle)
{
    return a_Handle->info;
}
unsigned GetGLHandle(const Device::Handle& a_Device, const Handle& a_Handle)
{
	return a_Handle->handle;
}
}