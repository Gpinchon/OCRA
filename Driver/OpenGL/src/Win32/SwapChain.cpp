#include <GL/Win32/SwapChain.hpp>
#include <GL/Win32/Surface.hpp>
#include <GL/Win32/Error.hpp>

#include <GL/Device.hpp>
#include <GL/Image/Image.hpp>
#include <GL/Queue/Queue.hpp>

#include <GL/glew.h>
#include <GL/wglew.h>

#include <dxgi1_3.h>
#include <d3d11.h>
#include <comdef.h>

namespace OCRA
{
static inline auto GetDXFormat(const Image::Format& a_Format)
{
    return DXGI_FORMAT_R8G8B8A8_UNORM;//TODO give the real Image Format
}
}

namespace OCRA::SwapChain::Win32
{
Impl::Impl(const Device::Handle& a_Device, const Info& a_Info)
    : SwapChain::Impl(a_Device, a_Info)
{
    if (info.oldSwapchain != nullptr) {
        auto win32SwapChain = std::static_pointer_cast<SwapChain::Win32::Impl>(info.oldSwapchain);
        deviceD3D = win32SwapChain->deviceD3D;
        deviceContextD3D = win32SwapChain->deviceContextD3D;
        swapChainD3D = win32SwapChain->swapChainD3D;
        glHandleD3D = win32SwapChain->glHandleD3D;
        renderTextureHandle = win32SwapChain->renderTextureHandle;
        info.oldSwapchain->Retire();
        info.oldSwapchain.reset();
        swapChainD3D->ResizeBuffers(
            info.minImageCount,
            info.imageExtent.width,
            info.imageExtent.height,
            GetDXFormat(info.imageFormat),
            0
        );
        return;
    }
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferDesc.Format = GetDXFormat(info.imageFormat);
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.BufferCount = info.minImageCount;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //TODO give the real Image Usage
    swapChainDesc.OutputWindow = HWND(info.surface->nativeWindow);
    swapChainDesc.Windowed = true;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    uint32_t flags = 0;
#ifdef DEBUG
    flags |= D3D11_CREATE_DEVICE_DEBUG;
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
        &swapChainD3D,            //SwapChain PTR
        &deviceD3D,               //Device PTR
        nullptr,                  //Feature Level
        &deviceContextD3D         //Immediat Context
    );
    glHandleD3D = wglDXOpenDeviceNV(deviceD3D);
    WIN32_CHECK_ERROR(glHandleD3D != nullptr);
    a_Device->PushCommand([this] {
        glGenTextures(1, &renderTextureHandle);
    }, true);
}

Impl::~Impl()
{
    if (glHandleD3D != nullptr) wglDXCloseDeviceNV(glHandleD3D);
    if (deviceD3D != nullptr) deviceD3D->Release();
    if (deviceContextD3D != nullptr) deviceContextD3D->Release();
    if (swapChainD3D != nullptr) swapChainD3D->Release();
}

void Impl::Retire()
{
    deviceD3D = nullptr;
    deviceContextD3D = nullptr;
    swapChainD3D = nullptr;
    glHandleD3D = nullptr;
    renderTextureHandle = 0;
}

void Impl::Present(const Queue::Handle& a_Queue, const uint32_t& a_ImageIndex)
{
    assert(!retired);
    ID3D11Texture2D* colorBuffer{ nullptr };
    ID3D11RenderTargetView* colorBufferView{ nullptr };
    GLuint  colorBufferNameGL{ 0 };
    swapChainD3D->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&colorBuffer);
    deviceD3D->CreateRenderTargetView(
        colorBuffer,
        &CD3D11_RENDER_TARGET_VIEW_DESC(D3D11_RTV_DIMENSION_TEXTURE2D, GetDXFormat(info.imageFormat)),
        &colorBufferView
    );
    D3D11_TEXTURE2D_DESC desc{};
    colorBuffer->GetDesc(&desc);
    a_Queue->PushCommand([this, imageIndex = a_ImageIndex, colorBuffer, colorBufferView, desc] {
        auto colorBufferHandle = wglDXRegisterObjectNV(glHandleD3D, colorBuffer, renderTextureHandle, GL_TEXTURE_2D, WGL_ACCESS_READ_WRITE_NV);
        WIN32_CHECK_ERROR(colorBufferHandle != nullptr);
        deviceContextD3D->OMSetRenderTargets(1, &colorBufferView, nullptr);
        wglDXLockObjectsNV(glHandleD3D, 1, &colorBufferHandle);
        glCopyImageSubData(
            images.at(imageIndex)->handle, GL_TEXTURE_2D, 0, 0, 0, 0,
            renderTextureHandle,           GL_TEXTURE_2D, 0, 0, 0, 0,
            desc.Width, desc.Height, 1);
        wglDXUnlockObjectsNV(glHandleD3D, 1, &colorBufferHandle);
        WIN32_CHECK_ERROR(!swapChainD3D->Present(0, 0));
        wglDXUnregisterObjectNV(glHandleD3D, colorBufferHandle);
    }, true);
    colorBuffer->Release();
    colorBufferView->Release();
}
}
