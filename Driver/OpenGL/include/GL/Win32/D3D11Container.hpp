#pragma once

#include <SwapChain.hpp>

#include <GL/Surface.hpp>
#include <GL/Win32/Error.hpp>
#include <GL/Win32/D3DContainerInterface.hpp>

#include <d3d11.h>

namespace OCRA::SwapChain::Win32
{
struct D3DContainer : D3DContainerInterface
{
    inline D3DContainer(const Info& a_Info) : D3DContainerInterface(a_Info)
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
        WIN32_CHECK_ERROR(S_OK == D3D11CreateDeviceAndSwapChain(
            nullptr,                  //Adapter
            D3D_DRIVER_TYPE_HARDWARE, //Driver Type
            nullptr,                  //Software
            flags,                    //Flags
            nullptr,                  //Feature Levels PTR
            0,                        //Feature Levels
            D3D11_SDK_VERSION,        //SDK Version
            &swapChainDesc,           //SwapChain Description
            &swapChain,               //SwapChain PTR
            (ID3D11Device**)&device,  //Device PTR
            nullptr,                  //Feature Level
            &deviceContext            //Immediat Context
        ));
        InitColorBuffer();
    }
    inline ~D3DContainer() {
        deviceContext->Release();
    }
    inline void ResizeBuffers(const Info& a_Info)
    {
        ResizeSwapChain(a_Info);
        InitColorBuffer();
    }
    inline void InitColorBuffer()
    {
        WIN32_CHECK_ERROR(S_OK == swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&colorBuffer));
        WIN32_CHECK_ERROR(colorBuffer != nullptr);
        D3D11_TEXTURE2D_DESC desc{};
        ((ID3D11Texture2D*)colorBuffer)->GetDesc(&desc);
        extent = { desc.Width, desc.Height };
    }
    inline void Present()
    {
        WIN32_CHECK_ERROR(S_OK == swapChain->Present(
            synchronize ? 1 : 0, 0
        ));
    }
    ID3D11DeviceContext*    deviceContext{ nullptr };
};
}