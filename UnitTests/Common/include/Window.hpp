#pragma once

#include <SwapChain.hpp>
#include <Common/Rect2D.hpp>
#include <Handle.hpp>

#include <chrono>
#include <functional>
#include <string>

OCRA_DECLARE_HANDLE(OCRA::Instance);
OCRA_DECLARE_HANDLE(OCRA::PhysicalDevice);
OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::Surface);
OCRA_DECLARE_HANDLE(OCRA::SwapChain);
OCRA_DECLARE_HANDLE(OCRA::Queue);
OCRA_DECLARE_HANDLE(OCRA::Queue::Fence);
OCRA_DECLARE_HANDLE(OCRA::Queue::Semaphore);

namespace OCRA {
class Window
{
public:
    Window(
        const Instance::Handle& a_Instance,
        const PhysicalDevice::Handle& a_PhysicalDevice,
        const Device::Handle& a_Device,
        const std::string& name, const uint32_t width, const uint32_t height);
    ~Window();
    void PushEvents();
    void Show();
    void Update();
    void Present(const Queue::Handle& a_Queue);
    Image::Handle AcquireNextImage(
        const std::chrono::nanoseconds& a_Timeout,
        const Queue::Semaphore::Handle& a_Semaphore = nullptr,
        const Queue::Fence::Handle& a_Fence = nullptr);
    
    auto IsClosing() const { return closing; }
    auto GetNativeHandle() const { return nativeHandle; }
    auto GetSurface() const { return surface; }
    auto GetSwapChain() const { return swapChain; }
    auto GetExtent() const { return extent; }
    auto GetSwapChainImageNbr() const { return swapChainImageNbr; }
    void SetSwapChainImageNbr(uint32_t a_ImageNbr);
    auto VSyncEnabled() const { return vSync; }
    void SetVSync(bool a_VSync);

    //reserved for system
    void ResizeCallback(const uint32_t, const uint32_t);
    //reserved for system
    void ClosingCallback() { closing = true; };

    std::function<void(const Window&, const uint32_t, const uint32_t)> OnResize;
    std::function<void(const Window&, const uint32_t, const uint32_t)> OnMinimize;
    std::function<void(const Window&, const uint32_t, const uint32_t)> OnMaximize;
    std::function<void(const Window&, const uint32_t, const uint32_t)> OnRestore;
    std::function<void(const Window&)> OnClose;
    std::function<void(const Window&)> OnPaint;
private:
    const void*             nativeHandle;
    bool                    created{ false };
    bool                    closing{ false };
    bool                    vSync{ true };
    Device::Handle          device;
    Surface::Handle         surface;
    SwapChain::Handle       swapChain;
    SwapChain::PresentInfo  presentInfo;
    uint32_t                swapChainImageNbr{ 3 };
    uExtent2D               extent;
};
}
