/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:31:50
*/
#pragma once

#include <OCRA/Core.hpp>

#include <stdexcept>

#include <GL/Device.hpp>
#include <GL/Image/Format.hpp>
#include <GL/Image/Image.hpp>
#include <GL/Image/View.hpp>
#include <GL/Common/Component.hpp>
#include <GL/glew.h>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA {
GLenum GetGLType(const ImageViewType& a_Type)
{
    switch (a_Type)
    {
    case ImageViewType::View1D:
        return GL_TEXTURE_1D;
    case ImageViewType::View2D:
        return GL_TEXTURE_2D;
    case ImageViewType::View3D:
        return GL_TEXTURE_3D;
    case ImageViewType::ViewCube:
        return GL_TEXTURE_CUBE_MAP;
    case ImageViewType::View1DArray:
        return GL_TEXTURE_1D_ARRAY;
    case ImageViewType::View2DArray:
        return GL_TEXTURE_2D_ARRAY;
    case ImageViewType::ViewCubeArray:
        return GL_TEXTURE_CUBE_MAP_ARRAY;
    default:
        throw std::runtime_error("Unknown Image View Type");
    }
}
}

namespace OCRA::Device {
Image::View::Handle Create(const Device::Handle& a_Device, const CreateImageViewInfo& a_Info)
{
    return std::make_shared<Image::View::Impl>(a_Device, a_Info);
}
}

namespace OCRA::Image::View {

static inline auto CreateImageView(const Device::Handle& a_Device, const CreateImageViewInfo& a_Info)
{
    uint32_t handle = 0;
    a_Device->PushCommand([&handle, a_Device, a_Info] {
        glGenTextures(1, &handle);
        glTextureView(
            handle,
            GetGLType(a_Info.type),
            a_Info.image->handle,
            GetGLSizedFormat(a_Info.format),
            a_Info.subRange.baseMipLevel,
            a_Info.subRange.levelCount,
            a_Info.subRange.baseArrayLayer,
            a_Info.subRange.layerCount);
        auto swizzleMask = GLSwizzleMask(a_Info.components);
        glTextureParameteriv(
            handle,
            GL_TEXTURE_SWIZZLE_RGBA,
            (GLint*)&swizzleMask);
        }, true);
    return handle;
}
Impl::Impl(const Device::Handle& a_Device, const CreateImageViewInfo& a_Info)
    : device(a_Device)
    , info(a_Info)
    , target(GetGLType(info.type))
    , format(GetGLSizedFormat(a_Info.format))
    , handle(CreateImageView(a_Device, a_Info))
{}
Impl::~Impl()
{
    device.lock()->PushCommand([handle = handle] {
        glDeleteTextures(1, &handle);
    }, false);
}
unsigned GetGLHandle(const Device::Handle& a_Device, const Handle& a_Handle)
{
    return a_Handle->handle;
}
}