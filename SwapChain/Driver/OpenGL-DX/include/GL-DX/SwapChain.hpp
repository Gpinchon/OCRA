#pragma once

#include <OCRA/Handle.hpp>
#include <OCRA/SwapChain.hpp>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);
OCRA_DECLARE_WEAK_HANDLE(OCRA::Queue);

namespace OCRA::SwapChain
{
struct Impl
{
    Impl(const Device::Handle& a_Device, const Info& a_Info);
    //retired SwapChains loose ownership of their FB and get unusable
    virtual void Retire() {
        retired = true;
    }
    virtual void Present(const Queue::Handle& a_Queue) = 0;
    virtual Image::Handle AcquireNextImage(
        const std::chrono::nanoseconds& a_TimeoutNS,
        const Semaphore::Handle& a_Semaphore,
        const Fence::Handle&     a_Fence) = 0;
    Info                        info;
    const Device::WeakHandle    device;
    bool                        retired{ false };
};
}