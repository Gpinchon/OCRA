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
    {
        WIN32_CHECK_ERROR(S_OK == CreateDXGIFactory2(0, __uuidof(IDXGIFactory2), &factory));
    }
    inline ~D3DContainerInterface() {
        colorBuffer->Release();
        swapChain->Release();
        device->Release();
        factory->Release();
    }
    inline void CreateSwapChain(const Info& a_Info)
    {
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
        std::memset(&swapChainDesc, sizeof(swapChainDesc), 0);
        swapChainDesc.format = format;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 1;
        swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED; //TODO give the real alpha mode
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;
        factory->CreateSwapChainForHwnd(
            device,                             //D3D Device
            HWND(a_Info.surface->nativeWindow), //Window Handle
            &swapChainDesc,                     //SwapChain Desc
            nullptr,                            //SwapChain Fullscreen Desc
            nullptr,                            //DXGI Output
            &swapChain                          //DXGI SwapChain PTR
        );
    }
    inline void ResizeSwapChain(const Info& a_Info)
    {
        if (colorBuffer != nullptr) colorBuffer->Release();
        swapChain->ResizeBuffers(
            1,
            a_Info.imageExtent.width,
            a_Info.imageExtent.height,
            format,
            0
        );
    }
    template<typename T>
    inline void GetColorBuffer()
    {
        WIN32_CHECK_ERROR(S_OK == swapChain->GetBuffer(0, __uuidof(T), (void**)&colorBuffer));
        WIN32_CHECK_ERROR(colorBuffer != nullptr);
    }
    inline void Present()
    {
        WIN32_CHECK_ERROR(S_OK == swapChain->Present(
            synchronize ? 1 : 0, 0
        ));
    }
    const bool          synchronize;
    const DXGI_FORMAT   format;
    IDXGIFactory2*      factory{ nullptr };
    IDXGISwapChain1*    swapChain{ nullptr };
    IUnknown*           device{ nullptr };
    IUnknown*           colorBuffer{ nullptr };
};
}