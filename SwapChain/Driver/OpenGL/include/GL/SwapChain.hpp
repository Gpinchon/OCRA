#pragma once

#include <OCRA/Handle.hpp>
#include <OCRA/SwapChain.hpp>

#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);
OCRA_DECLARE_WEAK_HANDLE(OCRA::Queue);
OCRA_DECLARE_WEAK_HANDLE(OCRA::Semaphore);

namespace OCRA::SwapChain
{
struct Impl
{
    virtual void Present(const Queue::Handle& a_Queue) = 0;
    virtual Image::Handle AcquireNextImage(
        const std::chrono::nanoseconds& a_TimeoutNS,
        const Semaphore::Handle& a_Semaphore,
        const Fence::Handle&     a_Fence) = 0;
    
};
}