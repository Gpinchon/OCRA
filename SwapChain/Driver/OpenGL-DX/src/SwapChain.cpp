#include <OCRA/SwapChain.hpp>
#include <OCRA/Image/Image.hpp>

#include <GL-DX/Win32/SwapChain.hpp>
#include <GL-DX/SwapChain.hpp>
#include <GL-DX/Surface.hpp>

#include <GL/Fence.hpp>
#include <GL/Semaphore.hpp>
#include <GL/Queue.hpp>
#include <GL/PhysicalDevice.hpp>
#include <GL/Device.hpp>
#include <GL/Image/Image.hpp>
#include <GL/Image/Format.hpp>
#include <GL/glew.h>

#include <stdexcept>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::SwapChain
{
Impl::Impl(const Device::Handle& a_Device, const Info& a_Info)
    : info(a_Info)
    , device(a_Device)
{}

Handle Create(const Device::Handle& a_Device, const Info& a_Info)
{
#ifdef _WIN32
    return Handle(new SwapChain::Win32::Impl(a_Device, a_Info));
#endif //_WIN32
}

void Present(const Queue::Handle& a_Queue, const PresentInfo& a_PresentInfo)
{
    for (const auto& semaphore : a_PresentInfo.waitSemaphores)
    {
        OCRA_ASSERT(semaphore->type == Semaphore::Type::Binary && "Cannot wait on Timeline Semaphores when presenting");
        std::static_pointer_cast<Semaphore::Binary>(semaphore)->Wait();
    }
    for (const auto& swapChain : a_PresentInfo.swapChains)
        swapChain->Present(a_Queue);
}

Image::Handle AcquireNextImage(
    const Device::Handle&           a_Device,
    const Handle&                   a_SwapChain,
    const std::chrono::nanoseconds& a_Timeout,
    const Semaphore::Handle& a_Semaphore,
    const Fence::Handle&     a_Fence)
{
    return a_SwapChain->AcquireNextImage(a_Timeout, a_Semaphore, a_Fence);
}
}