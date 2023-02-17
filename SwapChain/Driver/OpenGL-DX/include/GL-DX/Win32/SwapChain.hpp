#pragma once

#include <GL-DX/SwapChain.hpp>

#include <memory>

namespace OCRA::WGLDX {
struct DeviceMapping;
struct TextureMapping;
}

namespace OCRA::SwapChain::Win32 {
struct D3DContainer;
}

namespace OCRA::SwapChain::Win32
{
struct Impl : SwapChain::Impl
{
    Impl(const Device::Handle& a_Device, const Info& a_Info);
    ~Impl();
    //when retiring the SwapChain becomes "empty"
    virtual void Retire() override;
    virtual void Present(const Queue::Handle& a_Queue) override;
    virtual Image::Handle AcquireNextImage(
        const std::chrono::nanoseconds& a_Timeout,
        const Semaphore::Handle& a_Semaphore,
        const Fence::Handle&     a_Fence) override;
    
    std::unique_ptr<D3DContainer>                       d3dContainer;
    std::shared_ptr<WGLDX::DeviceMapping>               wglDXDeviceMapping;
    std::unique_ptr<WGLDX::TextureMapping>              wglDXTextureMapping;
    std::vector<Image::Handle>                          images;
    uint32_t                                            backBufferIndex{ 0 };
};
}