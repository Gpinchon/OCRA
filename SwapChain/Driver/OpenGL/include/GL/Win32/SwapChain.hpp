#pragma once

#include <GL/SwapChain.hpp>

#include <Common/Extent3D.hpp>
#include <Common/Vec2.hpp>

#include <GL/Common/WorkerThread.hpp>

#include <memory>

OCRA_DECLARE_HANDLE(OCRA::Image);

namespace OCRA::SwapChain::Win32 {
struct PresentShader {
    PresentShader();
    ~PresentShader();
    void Bind() const;
    void Unbind() const;
    uint32_t handle{ 0 };
};
struct PresentTexture {
    PresentTexture(const Image::Handle& a_FromImage);
    ~PresentTexture();
    void Bind() const;
    void Unbind() const;
    void UploadData() const;
    uint32_t handle{ 0 };
    uint32_t samplerHandle{ 0 };
    const uint32_t target{ 0 };
    const uint32_t dataType{ 0 };
    const uint32_t dataFormat{ 0 };
    const uint32_t internalFormat{ 0 };
    const Extent3D extent{ 0, 0, 0 };
};
struct PresentGeometry {
    PresentGeometry();
    ~PresentGeometry();
    void Bind() const;
    void Unbind() const;
    void Draw() const;
    uint32_t VBOhandle;
    uint32_t VAOhandle;
};
}

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
    WorkerThread                     workerThread;
    void*                            hglrc{ nullptr };
    void*                            hdc{ nullptr };
    std::unique_ptr<PresentShader>   presentShader;
    std::unique_ptr<PresentTexture>  presentTexture;
    std::unique_ptr<PresentGeometry> presentGeometry;
    std::vector<Image::Handle>       images;
    uint32_t                         backBufferIndex{ 0 };
};
}