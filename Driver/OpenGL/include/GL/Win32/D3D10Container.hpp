#pragma once

#include <SwapChain.hpp>

#include <GL/Surface.hpp>
#include <GL/Win32/Error.hpp>
#include <GL/Win32/D3DContainerInterface.hpp>

#include <d3d10.h>

namespace OCRA::SwapChain::Win32
{
struct D3DContainer : D3DContainerInterface
{
    inline D3DContainer(const Info& a_Info) : D3DContainerInterface(a_Info)
    {
        DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
        swapChainDesc.BufferDesc.Format = format;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.BufferCount = 1;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.OutputWindow = HWND(a_Info.surface->nativeWindow);
        swapChainDesc.Windowed = true;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;
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
            &swapChain,                //SwapChain PTR
            (ID3D10Device**)&device    //Device PTR
        ));
        GetColorBuffer(__uuidof(ID3D10Texture2D));
    }
    inline void ResizeBuffers(const Info& a_Info)
    {
        ResizeSwapChain(a_Info);
        GetColorBuffer(__uuidof(ID3D10Texture2D));
    }
    inline uiExtent2D GetExtent() const {
        D3D10_TEXTURE2D_DESC desc{};
        ((ID3D10Texture2D*)colorBuffer)->GetDesc(&desc);
        return { desc.Width, desc.Height };
    }
};
}