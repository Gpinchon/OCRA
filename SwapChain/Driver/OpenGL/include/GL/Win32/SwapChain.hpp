#pragma once

#include <GL/SwapChain.hpp>

#include <OCRA/Common/Extent3D.hpp>
#include <OCRA/Common/Vec2.hpp>

#include <GL/Common/WorkerThread.hpp>

#pragma once

#include <memory>

#include <GL/glew.h>

OCRA_DECLARE_HANDLE(OCRA::Image);

namespace OCRA::SwapChain::Win32 {
struct PresentShader;
struct PresentTexture;
struct PresentGeometry;
struct PresentPixels;
}

namespace OCRA::SwapChain::Win32
{
struct Impl : SwapChain::Impl
{
    Impl(const Device::Handle& a_Device, const Info& a_Info);
    ~Impl();
    //when retiring the SwapChain becomes "empty"
    //retired SwapChains loose ownership of their FB and get unusable
    void Retire();
    void PresentNV(const Queue::Handle& a_Queue, const std::vector<Queue::Semaphore::Handle>& a_WaitSemaphores);
    void PresentGL(const Queue::Handle& a_Queue, const std::vector<Queue::Semaphore::Handle>& a_WaitSemaphores);
    
    virtual void Present(const Queue::Handle& a_Queue, const std::vector<Queue::Semaphore::Handle>& a_WaitSemaphores) override;
    virtual Image::Handle AcquireNextImage(
        const std::chrono::nanoseconds& a_Timeout,
        const Queue::Semaphore::Handle& a_Semaphore,
        const Queue::Fence::Handle& a_Fence) override;

    const Device::WeakHandle    device;
    const Surface::Handle       surface{ nullptr };
    const PresentMode           presentMode;
    bool                        retired{ false };

    WorkerThread                     workerThread;
    void*                            hglrc{ nullptr };
    void*                            hdc{ nullptr };
    std::unique_ptr<PresentShader>   presentShader;
    std::unique_ptr<PresentTexture>  presentTexture;
    std::unique_ptr<PresentGeometry> presentGeometry;
    std::unique_ptr<PresentPixels>   presentPixels;
    std::vector<Image::Handle>       images;
    uint32_t                         backBufferIndex{ 0 };
};
}