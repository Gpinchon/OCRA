#include <GL/Win32/SwapChain.hpp>
#include <GL/Win32/Error.hpp>

#include <GL/Device.hpp>
#include <GL/Image/Image.hpp>
#include <GL/Queue/Queue.hpp>

#include <GL/glew.h>
#include <GL/wglew.h>

#ifdef USE_D3D11
#include <GL/Win32/D3D11Container.hpp>
#else
#include <GL/Win32/D3D10Container.hpp>
#endif //USE_D3D11

namespace OCRA::SwapChain::Win32
{
Impl::Impl(const Device::Handle& a_Device, const Info& a_Info)
    : SwapChain::Impl(a_Device, a_Info)
{
    if (info.oldSwapchain != nullptr) {
        auto win32SwapChain = std::static_pointer_cast<SwapChain::Win32::Impl>(info.oldSwapchain);
        d3dContainer = win32SwapChain->d3dContainer;
        glDeviceD3D = win32SwapChain->glDeviceD3D;
        glColorBufferD3D = win32SwapChain->glColorBufferD3D;
        renderTextureHandle = win32SwapChain->renderTextureHandle;
        info.oldSwapchain->Retire();
        info.oldSwapchain.reset();
        d3dContainer->ResizeBuffers(info);
        a_Device->PushCommand([this] {
            wglDXUnregisterObjectNV(glDeviceD3D, glColorBufferD3D);
            glColorBufferD3D = wglDXRegisterObjectNV(
                glDeviceD3D,
                d3dContainer->colorBuffer,
                renderTextureHandle,
                GL_TEXTURE_2D,
                WGL_ACCESS_WRITE_DISCARD_NV
            );
            WIN32_CHECK_ERROR(colorBufferHandle != nullptr);
        }, true);
        return;
    }
    d3dContainer = new D3DContainer(info);
    glDeviceD3D = wglDXOpenDeviceNV(d3dContainer->device);
    WIN32_CHECK_ERROR(glDeviceD3D != nullptr);
    a_Device->PushCommand([this] {
        glGenTextures(1, &renderTextureHandle);
        glColorBufferD3D = wglDXRegisterObjectNV(
            glDeviceD3D,
            d3dContainer->colorBuffer,
            renderTextureHandle,
            GL_TEXTURE_2D,
            WGL_ACCESS_WRITE_DISCARD_NV
        );
        WIN32_CHECK_ERROR(colorBufferHandle != nullptr);
    }, true);
}

Impl::~Impl()
{
    if (glColorBufferD3D != nullptr) {
        device.lock()->PushCommand([this]() {
            wglDXUnregisterObjectNV(glDeviceD3D, glColorBufferD3D);
        }, true);
    }
    if (renderTextureHandle != 0) {
        device.lock()->PushCommand([texture = renderTextureHandle]() {
            if (texture != 0) glDeleteTextures(1, &texture);
        }, false);
    }
    if (glDeviceD3D != nullptr) wglDXCloseDeviceNV(glDeviceD3D);
    if (d3dContainer != nullptr) delete d3dContainer;
}

void Impl::Present(const Queue::Handle& a_Queue, const uint32_t& a_ImageIndex)
{
    assert(!retired);
    a_Queue->PushCommand([this, imageIndex = a_ImageIndex]{
        wglDXLockObjectsNV(glDeviceD3D, 1, &colorBufferHandle);
        glCopyImageSubData(
            images.at(imageIndex)->handle, GL_TEXTURE_2D, 0, 0, 0, 0,
            renderTextureHandle,           GL_TEXTURE_2D, 0, 0, 0, 0,
            d3dContainer->extent.width, d3dContainer->extent.height, 1);
        wglDXUnlockObjectsNV(glDeviceD3D, 1, &colorBufferHandle);
    }, true);
    d3dContainer->Present();
}
}
