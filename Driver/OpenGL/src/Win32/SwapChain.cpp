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
static inline auto CreateImage(const Device::Handle& a_Device, const Info& a_Info)
{
    Image::Info imageInfo{};
    imageInfo.type = Image::Type::Image2D;
    imageInfo.extent.width = a_Info.imageExtent.width;
    imageInfo.extent.height = a_Info.imageExtent.height;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = a_Info.imageArrayLayers;
    imageInfo.format = a_Info.imageFormat;
    return Image::CreateEmpty(a_Device, imageInfo);
}
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
        images.push_back(Image::CreateEmpty(a_Device, imageInfo));
    }
    return images;
}

Impl::Impl(const Device::Handle& a_Device, const Info& a_Info)
    : SwapChain::Impl(a_Device, a_Info)
{
    images = CreateImages(a_Device, a_Info);
    if (info.oldSwapchain != nullptr) {
        auto win32SwapChain = std::static_pointer_cast<SwapChain::Win32::Impl>(info.oldSwapchain);
        images.swap(info.oldSwapchain->images);
        images.resize(info.minImageCount);
        for (auto i = win32SwapChain->info.minImageCount; i < info.minImageCount; ++i)
            images.push_back(CreateImage(a_Device, a_Info)); //Complete missing images
        d3dContainer.swap(win32SwapChain->d3dContainer);
        wglDXDeviceMapping.swap(win32SwapChain->wglDXDeviceMapping);
        info.oldSwapchain->Retire();
        info.oldSwapchain.reset();
        d3dContainer->ResizeBuffers(info);
        for (uint32_t i = 0; i < d3dContainer->colorBuffers.size(); ++i) {
            const auto& image = images.at(i);
            const auto& colorBuffer = d3dContainer->colorBuffers.at(i);
            wglDXTextureMappings.push_back(std::make_unique<WGLDX::TextureMapping>(wglDXDeviceMapping, colorBuffer, image->handle));
        }
        return;
    }
    d3dContainer = std::make_unique<D3DContainer>(info);
    wglDXDeviceMapping = std::make_shared<WGLDX::DeviceMapping>(a_Device, d3dContainer->device);
    for (uint32_t i = 0; i < d3dContainer->colorBuffers.size(); ++i) {
        const auto image = CreateImage(a_Device, a_Info);
        const auto& colorBuffer = d3dContainer->colorBuffers.at(i);
        images.push_back(image);
        wglDXTextureMappings.push_back(std::make_unique<WGLDX::TextureMapping>(wglDXDeviceMapping, colorBuffer, image->handle));
    }
}

Impl::~Impl()
{
}

void Impl::Retire() {
    d3dContainer = nullptr;
    wglDXTextureMappings.clear();
    wglDXDeviceMapping = nullptr;
    SwapChain::Impl::Retire();
}

void Impl::Present(const Queue::Handle& a_Queue, const uint32_t& a_ImageIndex)
{
    assert(!retired);
    const auto extent = d3dContainer->GetExtent();
    a_Queue->PushCommand([this, extent, imageIndex = a_ImageIndex]{
        wglDXTextureMappings.at(d3dContainer->GetCurrentBackBufferIndex())->Unlock();
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
