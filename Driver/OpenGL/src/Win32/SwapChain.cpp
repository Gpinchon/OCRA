#include <GL/Win32/SwapChain.hpp>
#include <GL/Win32/Error.hpp>

#include <GL/Device.hpp>
#include <GL/Image/Image.hpp>
#include <GL/Queue/Queue.hpp>
#include <GL/Queue/Semaphore.hpp>
#include <GL/Queue/Fence.hpp>

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
static inline auto CreateImages(const Device::Handle& a_Device, const Info& a_Info)
{
    std::vector<Image::Handle> images;
    for (auto i = 0u; i < a_Info.minImageCount; ++i)
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
{
    if (info.oldSwapchain != nullptr) {
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
    images = CreateImages(a_Device, a_Info);
    CreateTextureMappings();
}

Impl::~Impl()
{
}

void Impl::CreateTextureMappings()
{
    for (const auto& colorBuffer : d3dContainer->colorBuffers) {
        auto imageMapping = std::make_unique<WGLDX::TextureMapping>(wglDXDeviceMapping, colorBuffer);
        wglDXTextureMappings.push_back(imageMapping);
    }
}

void Impl::Retire() {
    wglDXTextureMappings.clear();
    wglDXDeviceMapping = nullptr;
    d3dContainer = nullptr;
    SwapChain::Impl::Retire();
}

void Impl::Present(const Queue::Handle& a_Queue, const uint32_t& a_ImageIndex)
{
    assert(!retired);
    const auto extent = d3dContainer->GetExtent();
    const auto currentBackBufferIndex = d3dContainer->GetCurrentBackBufferIndex();
    const auto& wglDXTextureMapping = wglDXTextureMappings.at(currentBackBufferIndex);
    a_Queue->PushCommand([this, extent, imageIndex = a_ImageIndex, wglDXTextureMapping]{
        glCopyImageSubData(
            images.at(imageIndex)->handle,        GL_TEXTURE_2D, 0, 0, 0, 0,
            wglDXTextureMapping->glTextureHandle, GL_TEXTURE_2D, 0, 0, 0, 0,
            extent.width, extent.height, 1);
        wglDXTextureMapping->Unlock();
    }, true);
    d3dContainer->Present();
#ifdef USE_D3D11 //Add glFinish frame delimiter for NSight
    a_Queue->PushCommand([] {
        glFinish();
    }, false);
#endif //USE_D3D11
}

//TODO: implement a timeout
uint32_t Impl::AcquireBackBuffer(
    const std::chrono::nanoseconds& a_Timeout,
    const Queue::Semaphore::Handle& a_Semaphore,
    const Queue::Fence::Handle& a_Fence)
{
    const auto currentBackBufferIndex = d3dContainer->GetCurrentBackBufferIndex();
    device.lock()->PushCommand([this, currentBackBufferIndex, semaphore = a_Semaphore, fence = a_Fence] {
        wglDXTextureMappings.at(currentBackBufferIndex)->Lock();
        if (semaphore != nullptr) {
            if (semaphore->type == Queue::Semaphore::Type::Binary)
                std::static_pointer_cast<Queue::Semaphore::Binary>(semaphore)->Signal();
            else throw std::runtime_error("Cannot wait on Timeline Semaphores when presenting");
        }
        if (fence != nullptr) fence->Signal();
    }, false);
    return currentBackBufferIndex;
}
}
