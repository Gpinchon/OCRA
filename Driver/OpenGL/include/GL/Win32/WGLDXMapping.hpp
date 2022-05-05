#pragma once

#include <GL/WeakHandle.hpp>
#include <GL/Device.hpp>
#include <GL/Win32/Error.hpp>

#include <GL/glew.h>
#include <GL/wglew.h>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::WGLDX
{
struct LockObject
{
    LockObject(HANDLE a_WGLDXDevice, HANDLE a_WGLDXObject)
        : wglDXDevice(a_WGLDXDevice)
        , wglDXObject(a_WGLDXObject)
    {
        WIN32_CHECK_ERROR(wglDXLockObjectsNV(wglDXDevice, 1, &wglDXObject));
    }
    ~LockObject()
    {
        WIN32_CHECK_ERROR(wglDXUnlockObjectsNV(wglDXDevice, 1, &wglDXObject));
    }
    HANDLE wglDXDevice, wglDXObject;
};

struct DeviceMapping
{
    DeviceMapping(const Device::Handle& a_Device, void* const a_D3DDevice)
        : device(a_Device)
        , d3dDevice(a_D3DDevice)
    {
        a_Device->PushCommand([this] {
            wglDXDevice = wglDXOpenDeviceNV(d3dDevice);
        }, true);
        WIN32_CHECK_ERROR(wglDXDevice != nullptr);
    }
    ~DeviceMapping()
    {
        device.lock()->PushCommand([this] {
            WIN32_CHECK_ERROR(wglDXCloseDeviceNV(wglDXDevice));
        }, true);
    }
    void* const         d3dDevice;
    Device::WeakHandle  device;
    HANDLE              wglDXDevice{ nullptr };
};

struct TextureMapping
{
    TextureMapping(DeviceMapping* const a_WGLDXDeviceMapping, void* const D3DColorBuffer)
        : wglDXDeviceMapping(a_WGLDXDeviceMapping)
    {
        wglDXDeviceMapping->device.lock()->PushCommand([this, D3DColorBuffer] {
            glGenTextures(1, &glTextureHandle);
            wglDXTextureHandle = wglDXRegisterObjectNV(
                wglDXDeviceMapping->wglDXDevice,
                D3DColorBuffer,
                glTextureHandle,
                GL_TEXTURE_2D,
                WGL_ACCESS_WRITE_DISCARD_NV
            );
        }, true);
        WIN32_CHECK_ERROR(wglDXTextureHandle != nullptr);
    }
    ~TextureMapping()
    {
        wglDXDeviceMapping->device.lock()->PushCommand([this] {
            WIN32_CHECK_ERROR(wglDXUnregisterObjectNV(wglDXDeviceMapping->wglDXDevice, wglDXTextureHandle));
            glDeleteTextures(1, &glTextureHandle);
        }, true);
    }
    LockObject Lock()
    {
        return { wglDXDeviceMapping->wglDXDevice, wglDXTextureHandle };
    }
    DeviceMapping* const wglDXDeviceMapping;
    uint32_t    glTextureHandle{ 0 };
    HANDLE      wglDXTextureHandle{ nullptr };
};
}