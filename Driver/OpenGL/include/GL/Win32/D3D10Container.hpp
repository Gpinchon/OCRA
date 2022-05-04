#pragma once

#include <SwapChain.hpp>

#include <GL/Surface.hpp>
#include <GL/Win32/DXGIFormat.hpp>
#include <GL/Win32/Error.hpp>

#include <d3d10.h>

namespace OCRA::SwapChain::Win32
{
struct D3DContainer
{
    inline D3DContainer(const Info& a_Info)
        : synchronize(a_Info.presentMode != PresentMode::Immediate)
        , format(GetDXFormat(a_Info.imageFormat, a_Info.imageColorSpace == Image::ColorSpace::sRGB))
    {
        DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
        swapChainDesc.BufferDesc.Format = format;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.BufferCount = a_Info.minImageCount;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //TODO give the real Image Usage
        swapChainDesc.OutputWindow = HWND(a_Info.surface->nativeWindow);
        swapChainDesc.Windowed = true;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
#ifdef DEBUG
        uint32_t flags = D3D10_CREATE_DEVICE_DEBUG;
#else
        uint32_t flags = 0;
#endif //DEBUG
        WIN32_CHECK_ERROR(S_OK == D3D10CreateDeviceAndSwapChain(
            nullptr,                   //Adapter
            D3D10_DRIVER_TYPE_HARDWARE,//Driver Type
            nullptr,                   //Software
            flags,                     //Flags
            D3D10_SDK_VERSION,         //SDK Version
            &swapChainDesc,            //SwapChain Description
            &swapChain,             //SwapChain PTR
            &device                 //Device PTR
        ));
        InitColorBuffer();
    }
    inline ~D3DContainer() {
        colorBufferView->Release();
        colorBuffer->Release();
        swapChain->Release();
        device->Release();
    }
    inline void ResizeBuffers(const Info& a_Info)
    {
        if (colorBufferView != nullptr) colorBufferView->Release();
        if (colorBuffer != nullptr) colorBuffer->Release();
        colorBufferView = nullptr;
        colorBuffer = nullptr;
        swapChain->ResizeBuffers(
            a_Info.minImageCount,
            a_Info.imageExtent.width,
            a_Info.imageExtent.height,
            format,
            0
        );
        InitColorBuffer();
    }
    inline void InitColorBuffer()
    {
        WIN32_CHECK_ERROR(S_OK == swapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (void**)&colorBuffer));
        WIN32_CHECK_ERROR(colorBuffer != nullptr);
        D3D10_RENDER_TARGET_VIEW_DESC rtvDescription{};
        rtvDescription.Format = format;
        rtvDescription.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
        rtvDescription.Texture2D.MipSlice = 0;
        WIN32_CHECK_ERROR(S_OK == device->CreateRenderTargetView(
            colorBuffer,
            &rtvDescription,
            &colorBufferView
        ));
        WIN32_CHECK_ERROR(colorBufferView != nullptr);
        device->OMSetRenderTargets(1, &colorBufferView, nullptr);
        D3D10_TEXTURE2D_DESC desc{};
        colorBuffer->GetDesc(&desc);
        extent = { desc.Width, desc.Height };
    }
    inline void Present()
    {
        WIN32_CHECK_ERROR(S_OK == swapChain->Present(
            synchronize ? 1 : 0, 0
        ));
    }
    const bool              synchronize;
    const DXGI_FORMAT       format;
    uiExtent2D              extent{ 0, 0 };
    IDXGISwapChain*         swapChain{ nullptr };
    //D3D10 specific members
    ID3D10Device*           device{ nullptr };
    ID3D10Texture2D*        colorBuffer{ nullptr };
    ID3D10RenderTargetView* colorBufferView{ nullptr };
};
}