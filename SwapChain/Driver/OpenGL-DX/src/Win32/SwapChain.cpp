#include <GL-DX/Win32/SwapChain.hpp>

#include <GL/Win32/Error.hpp>
#include <GL/Common/Assert.hpp>
#include <GL/Device.hpp>
#include <GL/Image/Image.hpp>
#include <GL/Queue/Queue.hpp>
#include <GL/Queue/Semaphore.hpp>
#include <GL/Queue/Fence.hpp>

#include <GL/glew.h>
#include <GL/wglew.h>

#ifdef OCRA_SWAPCHAIN_USE_D3D11
#include <GL-DX/Win32/D3D11Container.hpp>
#else
#include <GL-DX/Win32/D3D10Container.hpp>
#endif //USE_D3D11

#include <GL-DX/Win32/WGLDXMapping.hpp>

namespace OCRA::SwapChain::Win32
{
static inline auto CreateImages(const Device::Handle& a_Device, const Info& a_Info)
{
    std::vector<Image::Handle> images;
    for (auto i = 0u; i < a_Info.imageCount; ++i)
    {
        Image::Info imageInfo{};
        imageInfo.type = Image::Type::Image2D;
        imageInfo.extent.width = a_Info.imageExtent.width;
        imageInfo.extent.height = a_Info.imageExtent.height;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = a_Info.imageArrayLayers;
        imageInfo.format = a_Info.imageFormat;
        images.push_back(Image::Create(a_Device, imageInfo));
    }
    return images;
}

Impl::Impl(const Device::Handle& a_Device, const Info& a_Info)
    : SwapChain::Impl(a_Device, a_Info)
    , images(CreateImages(a_Device, a_Info))
{
    if (info.oldSwapchain != nullptr && !info.oldSwapchain->retired) {
        auto win32SwapChain = std::static_pointer_cast<SwapChain::Win32::Impl>(info.oldSwapchain);
        d3dContainer.swap(win32SwapChain->d3dContainer);
        wglDXDeviceMapping.swap(win32SwapChain->wglDXDeviceMapping);
        info.oldSwapchain->Retire();
        info.oldSwapchain.reset();
        d3dContainer->ResizeBuffers(info);
    }
    else {
        d3dContainer = std::make_unique<D3DContainer>(info);
        wglDXDeviceMapping = std::make_shared<WGLDX::DeviceMapping>(a_Device, d3dContainer->device);
    }
    wglDXTextureMapping = std::make_unique<WGLDX::TextureMapping>(wglDXDeviceMapping, d3dContainer->colorBuffer);
}

Impl::~Impl()
{
    Retire();
}

void Impl::Retire() {
    SwapChain::Impl::Retire();
    wglDXTextureMapping = nullptr;
    wglDXDeviceMapping = nullptr;
    d3dContainer = nullptr;
}

void Impl::Present(const Queue::Handle& a_Queue)
{
    OCRA_ASSERT(!retired);
    const auto extent = d3dContainer->GetExtent();
    a_Queue->PushCommand([this, extent]{
        wglDXTextureMapping->Lock();
        glCopyImageSubData(
            images.at(backBufferIndex)->handle,   GL_TEXTURE_2D, 0, 0, 0, 0,
            wglDXTextureMapping->glTextureHandle, GL_TEXTURE_2D, 0, 0, 0, 0,
            extent.width, extent.height, 1);
        wglDXTextureMapping->Unlock();
    }, true);
    d3dContainer->Present();
    backBufferIndex = (backBufferIndex + 1) % info.imageCount;
#ifdef USE_D3D11 //Add glFinish frame delimiter for NSight
    a_Queue->PushCommand([] {
        glFinish();
    }, false);
#endif //USE_D3D11
}

//TODO: implement a timeout
Image::Handle Impl::AcquireNextImage(
    const std::chrono::nanoseconds& a_Timeout,
    const Semaphore::Handle& a_Semaphore,
    const Fence::Handle& a_Fence)
{
    //We do not need to synchronize with the GPU for real here
    if (a_Semaphore != nullptr) {
        if (a_Semaphore->type == Semaphore::Type::Binary)
            std::static_pointer_cast<Semaphore::Binary>(a_Semaphore)->SignalNoSync();
        else throw std::runtime_error("Cannot wait on Timeline Semaphores when presenting");
    }
    if (a_Fence != nullptr) a_Fence->SignalNoSync();
    return images.at(backBufferIndex);
}
}
