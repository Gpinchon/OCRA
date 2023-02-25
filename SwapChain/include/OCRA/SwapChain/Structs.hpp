#pragma once

#include <OCRA/Core.hpp>
#include <OCRA/SwapChain/Handles.hpp>
#include <OCRA/SwapChain/Enums.hpp>

#include <vector>

namespace OCRA
{
struct SwapChainPresentInfo
{
    std::vector<Semaphore::Handle>  waitSemaphores;
    std::vector<SwapChain::Handle>  swapChains;
};
struct CreateSwapChainInfo {
    Surface::Handle       surface{ nullptr };
    uint32_t              imageCount{ 0 }; //You might get less depending on the implementation
    Format                imageFormat{ Format::Unknown };
    ColorSpace            imageColorSpace{ ColorSpace::Unknown };
    Extent2D              imageExtent{ 0, 0 };
    uint32_t              imageArrayLayers{ 0 };
    ImageUsageFlags       imageUsage{ ImageUsageFlagBits::None };
    SharingMode           imageSharingMode{ SharingMode::Exclusive };
    std::vector<uint32_t> queueFamilyIndices;
    SwapChainPresentMode  presentMode{ SwapChainPresentMode::Immediate };
    bool                  clipped{ false };
    SwapChain::Handle     oldSwapchain{ nullptr };
};
#ifdef _WIN32
struct CreateSurfaceInfo {
    void* hinstance;
    void* hwnd;
};
#endif
}
