#pragma once

#include <OCRA/Structs.hpp>
#include <OCRA/SwapChain/Core.hpp>

#include <GL/Common/WorkerThread.hpp>

#pragma once

#include <memory>

#include <GL/glew.h>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::SwapChain::Win32 {
struct PresentShader;
struct PresentTexture;
struct PresentGeometry;
struct PresentPixels;
}

namespace OCRA::SwapChain
{
struct Impl
{
    Impl(const Device::Handle& a_Device, const CreateSwapChainInfo& a_Info);
    ~Impl();
    //when retiring the SwapChain becomes "empty"
    //retired SwapChains loose ownership of their FB and get unusable
    void Retire();
    void PresentNV(const Queue::Handle& a_Queue);
    void PresentGL(const Queue::Handle& a_Queue);
    
    void Present(const Queue::Handle& a_Queue);

    const Device::WeakHandle    device;
    const Surface::Handle       surface{ nullptr };
    const SwapChainPresentMode  presentMode;
    bool                        retired{ false };

    WorkerThread                     workerThread;
    void*                            hglrc{ nullptr };
    void*                            hdc{ nullptr };
    std::unique_ptr<Win32::PresentShader>   presentShader;
    std::unique_ptr<Win32::PresentTexture>  presentTexture;
    std::unique_ptr<Win32::PresentGeometry> presentGeometry;
    std::unique_ptr<Win32::PresentPixels>   presentPixels;
    std::vector<Image::Handle>       images;
    uint32_t                         backBufferIndex{ 0 };
};
}