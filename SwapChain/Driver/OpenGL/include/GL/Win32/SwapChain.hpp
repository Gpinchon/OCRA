#pragma once

#include <GL/SwapChain.hpp>

#include <Common/Extent3D.hpp>
#include <Common/Vec2.hpp>

#include <GL/Common/WorkerThread.hpp>

#include <memory>

#include <GL/glew.h>

OCRA_DECLARE_HANDLE(OCRA::Image);

namespace OCRA::SwapChain::Win32 {
struct PresentShader;
struct PresentTexture;
struct PresentGeometry;
struct PresentPixels
{
    PresentPixels(const size_t& a_Size)
        : size(a_Size)
    {
        glGenBuffers(1, &handle);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, handle);
        glBufferStorage(GL_PIXEL_UNPACK_BUFFER, size * 3, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
        ptr = glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, size * 3, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    }
    ~PresentPixels() {
        glDeleteBuffers(1, &handle);
    }
    void Bind() {
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, handle);
    }
    void Unbind() {
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    }
    void Flush() {
        offset = offsetIndex * size;
        glFlushMappedBufferRange(GL_PIXEL_UNPACK_BUFFER, offset, size);
        ++offsetIndex %= 3;
    }
    void* GetPtr() {
        return (char*)ptr + offset;
    }
    uint32_t        handle;
    const size_t    size;
    void*           ptr;
    size_t          offsetIndex{ 0 };
    size_t          offset{ 0 };
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
    void PresentNV(const Queue::Handle& a_Queue);
    void PresentGL(const Queue::Handle& a_Queue);
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
    std::unique_ptr<PresentPixels>   presentPixels;
    std::vector<Image::Handle>       images;
    uint32_t                         backBufferIndex{ 0 };
};
}