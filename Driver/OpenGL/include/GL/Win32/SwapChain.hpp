#pragma once

#include <GL/SwapChain.hpp>

#include <d3d10.h>

namespace OCRA::SwapChain::Win32
{
struct Impl : SwapChain::Impl
{
    Impl(const Device::Handle& a_Device, const Info& a_Info);
    ~Impl();
    //when retiring the SwapChain becomes "empty"
    virtual void Retire() override {
        deviceD3D = nullptr;
        swapChainD3D = nullptr;
        glHandleD3D = nullptr;
        renderTextureHandle = 0;
    }
    virtual void Present(const Queue::Handle& a_Queue, const uint32_t& a_ImageIndex) override;
    const DXGI_FORMAT    formatD3D;;
    ID3D10Device*        deviceD3D{ nullptr };
    IDXGISwapChain*      swapChainD3D{ nullptr };
    HANDLE               glHandleD3D{ nullptr };
    uint32_t             renderTextureHandle{ 0 };
};
}