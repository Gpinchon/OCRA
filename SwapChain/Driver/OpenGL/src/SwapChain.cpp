#include <OCRA/SwapChain.hpp>
#include <OCRA/Image/Image.hpp>

#ifdef _WIN32
#include <GL/Win32/SwapChain.hpp>
#endif
#include <GL/SwapChain.hpp>
#include <GL/Surface.hpp>

#include <GL/Queue/Fence.hpp>
#include <GL/Queue/Semaphore.hpp>
#include <GL/Queue/Queue.hpp>
#include <GL/PhysicalDevice.hpp>
#include <GL/Device.hpp>
#include <GL/Image/Image.hpp>
#include <GL/Image/Format.hpp>
#include <GL/glew.h>

#include <cassert>
#include <stdexcept>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::SwapChain
{
Handle Create(const Device::Handle& a_Device, const Info& a_Info)
{
#ifdef _WIN32
    return Handle(new SwapChain::Win32::Impl(a_Device, a_Info));
#endif //_WIN32
}

void Present(const Queue::Handle& a_Queue, const PresentInfo& a_PresentInfo)
{
    for (const auto& semaphore : a_PresentInfo.waitSemaphores) {
#ifdef _DEBUG
        assert(semaphore->type == Queue::Semaphore::Type::Binary && "Cannot wait on Timeline Semaphores when presenting");
#endif
        std::static_pointer_cast<Queue::Semaphore::Binary>(semaphore)->Wait();
    }
    for (const auto& swapChain : a_PresentInfo.swapChains)
        swapChain->Present(a_Queue);
}

Image::Handle AcquireNextImage(
    const Device::Handle&           a_Device,
    const Handle&                   a_SwapChain,
    const std::chrono::nanoseconds& a_Timeout,
    const Queue::Semaphore::Handle& a_Semaphore,
    const Queue::Fence::Handle&     a_Fence)
{
    return a_SwapChain->AcquireNextImage(a_Timeout, a_Semaphore, a_Fence);
}
}