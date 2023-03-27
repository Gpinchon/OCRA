/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:31:50
*/
#pragma once

#include <OCRA/Core.hpp>

#include <GL/Device.hpp>
#include <GL/Enums.hpp>
#include <GL/Structs.hpp>
#include <GL/Image.hpp>
#include <GL/ImageView.hpp>

#include <GL/glew.h>

#include <stdexcept>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Device {
Image::View::Handle CreateImageView(const Device::Handle& a_Device, const CreateImageViewInfo& a_Info)
{
    return std::make_shared<Image::View::Impl>(a_Device, a_Info);
}
}

namespace OCRA::Image::View {
Impl::Impl(const Device::Handle& a_Device, const CreateImageViewInfo& a_Info)
    : device(a_Device)
    , info(a_Info)
    , target(GetGLImageViewType(info.type))
    , format(GetGLSizedFormat(a_Info.format))
{
    a_Device->PushCommand([this] {
        glGenTextures(1, &handle);
        glTextureView(
            handle,
            GetGLImageViewType(info.type),
            info.image->handle,
            GetGLSizedFormat(info.format),
            info.subRange.baseMipLevel,
            info.subRange.levelCount,
            info.subRange.baseArrayLayer,
            info.subRange.layerCount);
        auto swizzleMask = GLSwizzleMask(info.components);
        glTextureParameteriv(
            handle,
            GL_TEXTURE_SWIZZLE_RGBA,
            (GLint*)&swizzleMask);
    }, false);
}
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