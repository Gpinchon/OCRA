#pragma once

#include <GL/SwapChain.hpp>

#include <GL/Common/WorkerThread.hpp>

#include <memory>

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
        const Queue::Semaphore::Handle& a_Semaphore,
        const Queue::Fence::Handle&     a_Fence) override;
    WorkerThread               workerThread;
    void*                      hglrc{ nullptr };
    void*                      hdc{ nullptr };
    std::vector<unsigned char> pixelData;
    uint32_t                   textureHandle{ 0 };
    uint32_t                   textureInternalFormat{ 0 };
    uint32_t                   textureDataFormat{ 0 };
    uint32_t                   textureDataType{ 0 };
    uint32_t                   textureTarget{ 0 };
    uint32_t                   frameBufferHandle{ 0 };
    std::vector<Image::Handle> images;
    uint32_t                   backBufferIndex{ 0 };
};
}