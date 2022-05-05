#pragma once

#include <GL/WeakHandle.hpp>
#include <GL/Device.hpp>
#include <GL/Win32/D3DContainerInterface.hpp>

#include <GL/glew.h>
#include <GL/wglew.h>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::SwapChain::Win32
{
struct WGLDXDeviceMapping
{
    WGLDXDeviceMapping(const Device::Handle& a_Device, D3DContainerInterface* const a_D3DContainer)
        : device(a_Device)
        , d3dContainer(a_D3DContainer)
    {
        a_Device->PushCommand([this] {
            glDeviceD3D = wglDXOpenDeviceNV(d3dContainer->device);
        }, true);
    }
    ~WGLDXDeviceMapping()
    {
        device.lock()->PushCommand([this] {
            wglDXCloseDeviceNV(d3dContainer->device);
        }, true);
    }
    Device::WeakHandle device;
    D3DContainerInterface* const d3dContainer;
    HANDLE glDeviceD3D{ nullptr };
};

struct D3DTextureMapping
{
    D3DTextureMapping(WGLDXDeviceMapping* const a_WGLDXDeviceMapping, void* const D3DColorBuffer)
        : wglDXDeviceMapping(a_WGLDXDeviceMapping)
    {
        wglDXDeviceMapping->device.lock()->PushCommand([this, D3DColorBuffer] {
            glGenTextures(1, &glTextureHandle);
            glColorBufferD3D = wglDXRegisterObjectNV(
                wglDXDeviceMapping->glDeviceD3D,
                D3DColorBuffer,
                glTextureHandle,
                GL_TEXTURE_2D,
                WGL_ACCESS_WRITE_DISCARD_NV
            );
        }, true);
    }
    ~D3DTextureMapping()
    {
        wglDXDeviceMapping->device.lock()->PushCommand([this] {
            wglDXUnlockObjectsNV(wglDXDeviceMapping->glDeviceD3D, 1, (HANDLE*)glColorBufferD3D);
            glDeleteTextures(1, &glTextureHandle);
        }, true);
    }
    WGLDXDeviceMapping* const wglDXDeviceMapping;
    uint32_t glTextureHandle{ 0 };
    void* glColorBufferD3D{ nullptr };
};
}