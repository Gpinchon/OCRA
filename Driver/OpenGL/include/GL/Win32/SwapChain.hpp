#pragma once

#include <GL/SwapChain.hpp>

namespace OCRA::SwapChain::Win32
{
struct D3DContainer;
struct Impl : SwapChain::Impl
{
    Impl(const Device::Handle& a_Device, const Info& a_Info);
    ~Impl();
    //when retiring the SwapChain becomes "empty"
    virtual void Retire() override {
        d3dContainer = nullptr;
        glDeviceD3D = nullptr;
        glColorBufferD3D = nullptr;
        renderTextureHandle = 0;
    }
    virtual void Present(const Queue::Handle& a_Queue, const uint32_t& a_ImageIndex) override;
    D3DContainer*        d3dContainer;
    void*                glDeviceD3D{ nullptr };
    void*                glColorBufferD3D{ nullptr };
    uint32_t             renderTextureHandle{ 0 };
};
}