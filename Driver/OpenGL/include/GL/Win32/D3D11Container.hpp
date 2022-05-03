#pragma once

#include <GL/Surface.hpp>
#include <GL/Win32/DXGIFormat.hpp>
#include <GL/Win32/Error.hpp>

#include <d3d11.h>

namespace OCRA::SwapChain::Win32
{
struct PresentObject
{
    PresentObject(IDXGISwapChain* a_SwapChain, ID3D11Device* a_Device, ID3D11DeviceContext* a_DeviceContext, const DXGI_FORMAT& a_Format, bool a_Synchronize)
        : device(a_Device)
        , deviceContext(a_DeviceContext)
        , swapChain(a_SwapChain)
        , synchronize(a_Synchronize)
    {
        WIN32_CHECK_ERROR(S_OK == swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&colorBuffer));
        WIN32_CHECK_ERROR(colorBuffer != nullptr);
        D3D11_RENDER_TARGET_VIEW_DESC rtvDescription{};
        rtvDescription.Format = a_Format;
        rtvDescription.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        rtvDescription.Texture2D.MipSlice = 0;
        device->CreateRenderTargetView(
            colorBuffer,
            &rtvDescription,
            &colorBufferView
        );
        deviceContext->OMSetRenderTargets(1, &colorBufferView, nullptr);
    }
    ~PresentObject()
    {
        colorBuffer->Release();
        colorBufferView->Release();
        WIN32_CHECK_ERROR(S_OK == swapChain->Present(
            synchronize ? 1 : 0,
            0
        ));
    }
    inline uiExtent2D GetExtent() const {
        D3D11_TEXTURE2D_DESC desc{};
        colorBuffer->GetDesc(&desc);
        return { desc.Width, desc.Height };
    }
    inline auto GetColorBuffer() const {
        return colorBuffer;
    }
    const bool                  synchronize;
    IDXGISwapChain* const       swapChain;
    ID3D11Device* const         device;
    ID3D11DeviceContext* const  deviceContext;
    ID3D11Texture2D*            colorBuffer{ nullptr };
    ID3D11RenderTargetView*     colorBufferView{ nullptr };
};

struct D3DContainer
{
    D3DContainer(const Info& a_Info)
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
        uint32_t flags = D3D11_CREATE_DEVICE_DEBUG;
#else
        uint32_t flags = 0;
#endif //DEBUG
        D3D11CreateDeviceAndSwapChain(
            nullptr,                  //Adapter
            D3D_DRIVER_TYPE_HARDWARE, //Driver Type
            nullptr,                  //Software
            flags,                    //Flags
            nullptr,                  //Feature Levels PTR
            0,                        //Feature Levels
            D3D11_SDK_VERSION,        //SDK Version
            &swapChainDesc,           //SwapChain Description
            &swapChain,               //SwapChain PTR
            &device,                  //Device PTR
            nullptr,                  //Feature Level
            &deviceContext            //Immediat Context
        );
    }
    ~D3DContainer() {
        swapChain->Release();
        device->Release();
    }
    inline auto CreatePresentObject()
    {
        return PresentObject(swapChain, device, deviceContext, format, synchronize);
    }
    inline void ResizeBuffers(const Info& a_Info)
    {
        swapChain->ResizeBuffers(
            a_Info.minImageCount,
            a_Info.imageExtent.width,
            a_Info.imageExtent.height,
            format,
            0
        );
    }
    const bool           synchronize;
    const DXGI_FORMAT    format;
    IDXGISwapChain*      swapChain{ nullptr };
    ID3D11Device*        device{ nullptr };
    ID3D11DeviceContext* deviceContext{ nullptr };
};
}