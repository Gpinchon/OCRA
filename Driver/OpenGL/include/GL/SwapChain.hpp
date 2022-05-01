#pragma once

#include <Handle.hpp>
#include <SwapChain.hpp>

#include <GL/WeakHandle.hpp>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);
OCRA_DECLARE_WEAK_HANDLE(OCRA::Queue);

namespace OCRA::SwapChain
{
struct Impl
{
    Impl(const Device::Handle& a_Device, const Info& a_Info);
    ~Impl();
    void Retire() {
        retired = true;
        frameBufferHandle = 0;
    }
    virtual void Present(const Queue::Handle& a_Queue, const uint32_t& a_ImageIndex) = 0;
    Info                        info;
    const Device::WeakHandle    device;
    bool                        retired{ false };
    std::vector<Image::Handle>  images;
    Extent2D                    realExtent;
    uint32_t                    frameBufferHandle;
};
}