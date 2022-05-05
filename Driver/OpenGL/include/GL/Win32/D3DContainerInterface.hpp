#pragma once

#include <SwapChain.hpp>

#include <Common/Extent2D.hpp>
#include <GL/Win32/DXGIFormat.hpp>
#include <GL/Win32/Error.hpp>

#include <dxgi1_3.h>

namespace OCRA::SwapChain::Win32
{
struct D3DContainerInterface
{
    inline D3DContainerInterface(const Info& a_Info)
        : synchronize(a_Info.presentMode != PresentMode::Immediate)
        , format(GetDXFormat(a_Info.imageFormat, a_Info.imageColorSpace == Image::ColorSpace::sRGB))
    {}
    inline ~D3DContainerInterface() {
        colorBuffer->Release();
        device->Release();
        swapChain->Release();
    }
    inline void ResizeSwapChain(const Info& a_Info)
    {
        if (colorBuffer != nullptr) colorBuffer->Release();
        colorBuffer = nullptr;
        swapChain->ResizeBuffers(
            a_Info.minImageCount,
            a_Info.imageExtent.width,
            a_Info.imageExtent.height,
            format,
            0
        );
        DXGI_SWAP_CHAIN_DESC swapChainDesc;
        swapChain->GetDesc(&swapChainDesc);
    }
    inline void Present()
    {
        WIN32_CHECK_ERROR(S_OK == swapChain->Present(
            synchronize ? 1 : 0, 0
        ));
    }
    const bool          synchronize;
    const DXGI_FORMAT   format;
    uiExtent2D          extent{ 0, 0 };
    IDXGISwapChain*     swapChain{ nullptr };
    IUnknown* device{ nullptr };
    IUnknown* colorBuffer{ nullptr };
};
}