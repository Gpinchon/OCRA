#pragma once

#include <SwapChain.hpp>

#include <GL/Surface.hpp>
#include <GL/Win32/Error.hpp>
#include <GL/Win32/D3DContainerInterface.hpp>

#include <d3d11.h>

#include <cassert>

namespace OCRA::SwapChain::Win32
{
struct D3DContainer : D3DContainerInterface
{
    inline D3DContainer(const Info& a_Info) : D3DContainerInterface(a_Info)
    {
#ifdef DEBUG
        uint32_t flags = D3D11_CREATE_DEVICE_DEBUG;
#else
        uint32_t flags = 0;
#endif //DEBUG
        WIN32_CHECK_ERROR(S_OK == D3D11CreateDevice(
            nullptr,                  //Adapter
            D3D_DRIVER_TYPE_HARDWARE, //Driver Type
            nullptr,                  //Software
            flags,                    //Flags
            nullptr,                  //Feature Levels PTR
            0,                        //Feature Levels Nbr
            D3D11_SDK_VERSION,        //SDK Version
            (ID3D11Device**)&device,  //Device PTR
            nullptr,                  //Feature Level
            nullptr                   //Device Context PTR
        ));
        CreateSwapChain(a_Info);
        GetBuffers<ID3D11Texture2D>();
    }
    inline void ResizeBuffers(const Info& a_Info)
    {
        ResizeSwapChain(a_Info);
        GetBuffers<ID3D11Texture2D>();
    }
};
}