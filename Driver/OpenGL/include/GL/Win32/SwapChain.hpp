#pragma once

#include <GL/SwapChain.hpp>

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
    virtual void Present(const Queue::Handle& a_Queue, const uint32_t& a_ImageIndex) override;
    virtual uint32_t AcquireBackBuffer(
        const std::chrono::nanoseconds& a_Timeout,
        const Queue::Semaphore::Handle& a_Semaphore,
        const Queue::Fence::Handle&     a_Fence) override;
    std::unique_ptr<D3DContainer>                       d3dContainer;
    std::shared_ptr<WGLDX::DeviceMapping>               wglDXDeviceMapping;
    std::vector<std::unique_ptr<WGLDX::TextureMapping>> wglDXTextureMappings;
};
}