#pragma once

#include <SwapChain.hpp>

#include <GL-DX/Surface.hpp>
#include <GL-DX/Win32/Error.hpp>
#include <GL-DX/Win32/D3DContainerInterface.hpp>

#include <d3d10.h>

namespace OCRA::SwapChain::Win32
{
struct D3DContainer : D3DContainerInterface
{
    inline D3DContainer(const Info& a_Info) : D3DContainerInterface(a_Info)
    {
#ifdef _DEBUG
        uint32_t flags = D3D10_CREATE_DEVICE_DEBUG;
#else
        uint32_t flags = 0;
#endif _DEBUG
        WIN32_CHECK_ERROR(S_OK == D3D10CreateDevice(
            nullptr,                   //Adapter
            D3D10_DRIVER_TYPE_HARDWARE,//Driver Type
            nullptr,                   //Software
            flags,                     //Flags
            D3D10_SDK_VERSION,         //SDK Version
            (ID3D10Device**)&device    //Device PTR
        ));
        CreateSwapChain(a_Info);
        GetBuffer<ID3D10Texture2D>();
    }
    inline void ResizeBuffers(const Info& a_Info)
    {
        ResizeSwapChain(a_Info);
        GetBuffer<ID3D10Texture2D>();
    }
};
}