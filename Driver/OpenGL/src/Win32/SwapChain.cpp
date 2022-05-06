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

#include <GL/Win32/WGLDXMapping.hpp>

namespace OCRA::SwapChain::Win32
{
Impl::Impl(const Device::Handle& a_Device, const Info& a_Info)
    : SwapChain::Impl(a_Device, a_Info)
{
    if (info.oldSwapchain != nullptr) {
        auto win32SwapChain = std::static_pointer_cast<SwapChain::Win32::Impl>(info.oldSwapchain);
        d3dContainer.swap(win32SwapChain->d3dContainer);
        wglDXDeviceMapping.swap(win32SwapChain->wglDXDeviceMapping);
        info.oldSwapchain->Retire();
        info.oldSwapchain.reset();
        d3dContainer->ResizeBuffers(info);
        wglDXTextureMapping = std::make_unique<WGLDX::TextureMapping>(wglDXDeviceMapping, d3dContainer->colorBuffer);
        return;
    }
    d3dContainer = std::make_unique<D3DContainer>(info);
    wglDXDeviceMapping = std::make_shared<WGLDX::DeviceMapping>(a_Device, d3dContainer->device);
    wglDXTextureMapping = std::make_unique<WGLDX::TextureMapping>(wglDXDeviceMapping, d3dContainer->colorBuffer);
}

Impl::~Impl()
{
}

void Impl::Retire() {
    d3dContainer = nullptr;
    wglDXDeviceMapping = nullptr;
    wglDXTextureMapping = nullptr;
    SwapChain::Impl::Retire();
}

void Impl::Present(const Queue::Handle& a_Queue, const uint32_t& a_ImageIndex)
{
    assert(!retired);
    const auto extent = d3dContainer->GetExtent();
    a_Queue->PushCommand([this, extent, imageIndex = a_ImageIndex]{
        const auto lock = wglDXTextureMapping->Lock();
        glCopyImageSubData(
            images.at(imageIndex)->handle,        GL_TEXTURE_2D, 0, 0, 0, 0,
            wglDXTextureMapping->glTextureHandle, GL_TEXTURE_2D, 0, 0, 0, 0,
            extent.width, extent.height, 1);
        d3dContainer->Present();
    }, d3dContainer->synchronize);
    
}
}
