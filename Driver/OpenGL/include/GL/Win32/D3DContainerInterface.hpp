#pragma once

#include <SwapChain.hpp>

#include <Common/Extent2D.hpp>
#include <GL/Win32/DXGIFormat.hpp>
#include <GL/Win32/Error.hpp>

#include <dxgi1_3.h>

#pragma comment(lib , "dxgi") 

namespace OCRA::SwapChain::Win32
{
IDXGIFactory* GetDeviceFactory(IUnknown* device)
{
    IDXGIDevice* dxgiDevice = 0;
    IDXGIAdapter* dxgiAdapter = 0;
    IDXGIFactory* dxgiFactory = 0;

    if (SUCCEEDED(device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice)))
    {
        if (SUCCEEDED(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter)))
        {
            dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
            dxgiAdapter->Release();
        }
        dxgiDevice->Release();
    }
    WIN32_CHECK_ERROR(dxgiDevice != nullptr);
    return dxgiFactory;
}
struct D3DContainerInterface
{
    inline D3DContainerInterface(const Info& a_Info)
        : synchronize(a_Info.presentMode != PresentMode::Immediate)
        , format(GetDXFormat(a_Info.imageFormat, a_Info.imageColorSpace == Image::ColorSpace::sRGB))
    {
    }
    inline ~D3DContainerInterface() {
        for (auto& colorBuffer : colorBuffers) colorBuffer->Release();
        swapChain->Release();
        device->Release();
    }
    inline void CreateSwapChain(const Info& a_Info)
    {
        auto factory = GetDeviceFactory(device);
        DXGI_SWAP_CHAIN_DESC desc{};
        std::memset(&desc, sizeof(desc), 0);
        desc.BufferDesc.Format = format;
        desc.BufferDesc.Width = a_Info.imageExtent.width;
        desc.BufferDesc.Height = a_Info.imageExtent.height;
        desc.SampleDesc.Count = 1;
        desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.BufferCount = a_Info.minImageCount;
        desc.OutputWindow = HWND(a_Info.surface->nativeWindow);
        desc.Windowed = true;
        desc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;
        factory->CreateSwapChain(
            device,
            &desc,
            &swapChain
        );
        WIN32_CHECK_ERROR(swapChain != nullptr);
        factory->Release();
    }
    inline void ResizeSwapChain(const Info& a_Info)
    {
        for (auto& colorBuffer : colorBuffers) colorBuffer->Release();
        colorBuffers.clear();
        swapChain->ResizeBuffers(
            1,
            a_Info.imageExtent.width,
            a_Info.imageExtent.height,
            format,
            0
        );
        currentBackBuffer = 0;
    }
    template<typename T>
    inline auto GetBuffers()
    {
        DXGI_SWAP_CHAIN_DESC desc{};
        swapChain->GetDesc(&desc);
        colorBuffers.resize(desc.BufferCount);
        for (auto i = 0u; i < desc.BufferCount; ++i) {
            T* buffer = nullptr;
            WIN32_CHECK_ERROR(S_OK == swapChain->GetBuffer(i, __uuidof(T), (void**)&buffer));
            WIN32_CHECK_ERROR(buffer != nullptr);
            colorBuffers.at(i) = buffer;
        }
    }
    inline void Present()
    {
        const auto swapChainStatus = swapChain->Present(synchronize ? 1 : 0, 0);
        WIN32_CHECK_ERROR(S_OK == swapChainStatus || DXGI_STATUS_OCCLUDED == swapChainStatus);
        currentBackBuffer = (currentBackBuffer + 1) % colorBuffers.size();
    }
    inline uExtent2D GetExtent()
    {
        DXGI_SWAP_CHAIN_DESC desc{};
        swapChain->GetDesc(&desc);
        return { desc.BufferDesc.Width, desc.BufferDesc.Height };
    }
    const bool             synchronize;
    const DXGI_FORMAT      format;
    uint32_t               currentBackBuffer = 0;
    IDXGISwapChain*        swapChain{ nullptr };
    IUnknown*              device{ nullptr };
    std::vector<IUnknown*> colorBuffers;
};
}