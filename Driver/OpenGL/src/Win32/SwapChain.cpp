#include <GL/Win32/SwapChain.hpp>
#include <GL/Win32/Surface.hpp>
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
        glHandleD3D = win32SwapChain->glHandleD3D;
        renderTextureHandle = win32SwapChain->renderTextureHandle;
        info.oldSwapchain->Retire();
        info.oldSwapchain.reset();
        d3dContainer->ResizeBuffers(info);
        return;
    }
    a_Device->PushCommand([this] {
        glGenTextures(1, &renderTextureHandle);
    }, true);
    d3dContainer = new D3DContainer(info);
    glHandleD3D = wglDXOpenDeviceNV(d3dContainer->device);
    WIN32_CHECK_ERROR(glHandleD3D != nullptr);
}

Impl::~Impl()
{
    if (glHandleD3D != nullptr) wglDXCloseDeviceNV(glHandleD3D);
    if (d3dContainer != nullptr) delete d3dContainer;
    if (renderTextureHandle != 0) {
        device.lock()->PushCommand([texture = renderTextureHandle]() {
            glDeleteTextures(1, &texture);
        }, true);
    }
}

void Impl::Present(const Queue::Handle& a_Queue, const uint32_t& a_ImageIndex)
{
    assert(!retired);
    const auto presentObject = PresentObject(d3dContainer);
    a_Queue->PushCommand([this, imageIndex = a_ImageIndex, colorBuffer = presentObject.GetColorBuffer(), extent = presentObject.GetExtent()]{
        auto colorBufferHandle = wglDXRegisterObjectNV(glHandleD3D, colorBuffer, renderTextureHandle, GL_TEXTURE_2D, WGL_ACCESS_WRITE_DISCARD_NV);
        WIN32_CHECK_ERROR(colorBufferHandle != nullptr);
        wglDXLockObjectsNV(glHandleD3D, 1, &colorBufferHandle);
        glCopyImageSubData(
            images.at(imageIndex)->handle, GL_TEXTURE_2D, 0, 0, 0, 0,
            renderTextureHandle,           GL_TEXTURE_2D, 0, 0, 0, 0,
            extent.width, extent.height, 1);
        wglDXUnlockObjectsNV(glHandleD3D, 1, &colorBufferHandle);
        wglDXUnregisterObjectNV(glHandleD3D, colorBufferHandle);
    }, true);
}
}
