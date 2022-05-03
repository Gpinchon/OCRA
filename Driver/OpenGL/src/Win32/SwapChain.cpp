#include <GL/Win32/SwapChain.hpp>
#include <GL/Win32/Surface.hpp>
#include <GL/Win32/Error.hpp>

#include <GL/Device.hpp>
#include <GL/Image/Image.hpp>
#include <GL/Queue/Queue.hpp>

#include <GL/glew.h>
#include <GL/wglew.h>

#include <dxgi1_3.h>
#include <d3d10.h>
#include <comdef.h>

namespace OCRA
{
static inline auto GetDXFormat(const Image::Format& a_Format, bool a_SRGB)
{
    switch (a_Format)
    {
    case Image::Format::Uint8_Normalized_R:
        return DXGI_FORMAT_R8_UNORM;
    case Image::Format::Uint8_Normalized_RG:
        return DXGI_FORMAT_R8G8_UNORM;
    case Image::Format::Uint8_Normalized_RGBA:
        return a_SRGB ? DXGI_FORMAT_R8G8B8A8_UNORM_SRGB : DXGI_FORMAT_R8G8B8A8_UNORM;
    case Image::Format::Int8_Normalized_R:
        return DXGI_FORMAT_R8_SNORM;
    case Image::Format::Int8_Normalized_RG:
        return DXGI_FORMAT_R8G8_SNORM;
    case Image::Format::Int8_Normalized_RGBA:
        return DXGI_FORMAT_R8G8B8A8_SNORM;
    case Image::Format::Uint8_R:
        return DXGI_FORMAT_R8_UINT;
    case Image::Format::Uint8_RG:
        return DXGI_FORMAT_R8G8_UINT;
    case Image::Format::Uint8_RGBA:
        return DXGI_FORMAT_R8G8B8A8_UINT;
    case Image::Format::Int8_R:
        return DXGI_FORMAT_R8_SINT;
    case Image::Format::Int8_RG:
        return DXGI_FORMAT_R8G8_SINT;
    case Image::Format::Int8_RGBA:
        return DXGI_FORMAT_R8G8B8A8_SINT;
    case Image::Format::Uint16_Normalized_R:
        return DXGI_FORMAT_R16_UNORM;
    case Image::Format::Uint16_Normalized_RG:
        return DXGI_FORMAT_R16G16_UNORM;
    case Image::Format::Uint16_Normalized_RGBA:
        return DXGI_FORMAT_R16G16B16A16_UNORM;
    case Image::Format::Int16_Normalized_R:
        return DXGI_FORMAT_R16_SNORM;
    case Image::Format::Int16_Normalized_RG:
        return DXGI_FORMAT_R16G16_SNORM;
    case Image::Format::Int16_Normalized_RGBA:
        return DXGI_FORMAT_R16G16B16A16_SNORM;
    case Image::Format::Uint16_R:
        return DXGI_FORMAT_R16_UINT;
    case Image::Format::Uint16_RG:
        return DXGI_FORMAT_R16G16_UINT;
    case Image::Format::Uint16_RGBA:
        return DXGI_FORMAT_R16G16B16A16_UINT;
    case Image::Format::Int16_R:
        return DXGI_FORMAT_R16_SINT;
    case Image::Format::Int16_RG:
        return DXGI_FORMAT_R16G16_SINT;
    case Image::Format::Int16_RGBA:
        return DXGI_FORMAT_R16G16B16A16_SINT;
    case Image::Format::Uint32_R:
        return DXGI_FORMAT_R32_UINT;
    case Image::Format::Uint32_RG:
        return DXGI_FORMAT_R32G32_UINT;
    case Image::Format::Uint32_RGB:
        return DXGI_FORMAT_R32G32B32_UINT;
    case Image::Format::Uint32_RGBA:
        return DXGI_FORMAT_R32G32B32A32_UINT;
    case Image::Format::Int32_R:
        return DXGI_FORMAT_R32_SINT;
    case Image::Format::Int32_RG:
        return DXGI_FORMAT_R32G32_SINT;
    case Image::Format::Int32_RGB:
        return DXGI_FORMAT_R32G32B32_SINT;
    case Image::Format::Int32_RGBA:
        return DXGI_FORMAT_R32G32B32A32_SINT;
    case Image::Format::Float16_R:
        return DXGI_FORMAT_R16_FLOAT;
    case Image::Format::Float16_RG:
        return DXGI_FORMAT_R16G16_FLOAT;
    case Image::Format::Float16_RGBA:
        return DXGI_FORMAT_R16G16B16A16_FLOAT;
    case Image::Format::Float32_R:
        return DXGI_FORMAT_R32_FLOAT;
    case Image::Format::Float32_RG:
        return DXGI_FORMAT_R32G32_FLOAT;
    case Image::Format::Float32_RGB:
        return DXGI_FORMAT_R32G32B32_FLOAT;
    case Image::Format::Float32_RGBA:
        return DXGI_FORMAT_R32G32B32A32_FLOAT;
    case Image::Format::Uint16_Normalized_Depth:
        return DXGI_FORMAT_D16_UNORM;
    case Image::Format::Float32_Depth:
        return DXGI_FORMAT_D32_FLOAT;
    case Image::Format::Uint24_Normalized_Depth_Uint8_Stencil:
        return DXGI_FORMAT_D24_UNORM_S8_UINT;
    case Image::Format::Float32_Normalized_Depth_Uint8_Stencil:
        return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
    default:
        throw std::runtime_error("Incompatible Image Format");
    }
}
}

namespace OCRA::SwapChain::Win32
{
Impl::Impl(const Device::Handle& a_Device, const Info& a_Info)
    : SwapChain::Impl(a_Device, a_Info)
    , formatD3D(GetDXFormat(info.imageFormat, info.imageColorSpace == Image::ColorSpace::sRGB))
{
    if (info.oldSwapchain != nullptr) {
        auto win32SwapChain = std::static_pointer_cast<SwapChain::Win32::Impl>(info.oldSwapchain);
        deviceD3D = win32SwapChain->deviceD3D;
        swapChainD3D = win32SwapChain->swapChainD3D;
        glHandleD3D = win32SwapChain->glHandleD3D;
        renderTextureHandle = win32SwapChain->renderTextureHandle;
        info.oldSwapchain->Retire();
        info.oldSwapchain.reset();
        swapChainD3D->ResizeBuffers(
            info.minImageCount,
            info.imageExtent.width,
            info.imageExtent.height,
            formatD3D,
            0
        );
        return;
    }
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferDesc.Format = formatD3D;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.BufferCount = info.minImageCount;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //TODO give the real Image Usage
    swapChainDesc.OutputWindow = HWND(info.surface->nativeWindow);
    swapChainDesc.Windowed = true;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
#ifdef DEBUG
    uint32_t flags = D3D10_CREATE_DEVICE_DEBUG;
#else
    uint32_t flags = 0;
#endif //DEBUG
    D3D10CreateDeviceAndSwapChain(
        nullptr,                  //Adapter
        D3D_DRIVER_TYPE_HARDWARE, //Driver Type
        nullptr,                  //Software
        flags,                    //Flags
        D3D10_SDK_VERSION,        //SDK Version
        &swapChainDesc,           //SwapChain Description
        &swapChainD3D,            //SwapChain PTR
        &deviceD3D,               //Device PTR
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
    if (swapChainD3D != nullptr) swapChainD3D->Release();
    if (deviceD3D != nullptr) deviceD3D->Release();
    if (renderTextureHandle != 0) {
        device.lock()->PushCommand([texture = renderTextureHandle] {
            glGenTextures(1, &texture);
        }, true);
    }
}

void Impl::Present(const Queue::Handle& a_Queue, const uint32_t& a_ImageIndex)
{
    assert(!retired);
    ID3D10Texture2D*        colorBuffer{ nullptr };
    ID3D10RenderTargetView* colorBufferView{ nullptr };
    swapChainD3D->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&colorBuffer);
    D3D10_RENDER_TARGET_VIEW_DESC rtvDescription;
    rtvDescription.Format = formatD3D;
    rtvDescription.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
    rtvDescription.Texture2D.MipSlice = 0;
    deviceD3D->CreateRenderTargetView(
        colorBuffer,
        &rtvDescription,
        &colorBufferView
    );
    deviceD3D->OMSetRenderTargets(1, &colorBufferView, nullptr);
    D3D10_TEXTURE2D_DESC desc{};
    colorBuffer->GetDesc(&desc);
    a_Queue->PushCommand([this, imageIndex = a_ImageIndex, colorBuffer, desc] {
        auto colorBufferHandle = wglDXRegisterObjectNV(glHandleD3D, colorBuffer, renderTextureHandle, GL_TEXTURE_2D, WGL_ACCESS_WRITE_ONLY_NV);
        WIN32_CHECK_ERROR(colorBufferHandle != nullptr);
        wglDXLockObjectsNV(glHandleD3D, 1, &colorBufferHandle);
        glCopyImageSubData(
            images.at(imageIndex)->handle, GL_TEXTURE_2D, 0, 0, 0, 0,
            renderTextureHandle,           GL_TEXTURE_2D, 0, 0, 0, 0,
            desc.Width, desc.Height, 1);
        wglDXUnlockObjectsNV(glHandleD3D, 1, &colorBufferHandle);
        wglDXUnregisterObjectNV(glHandleD3D, colorBufferHandle);
    }, true);
    colorBuffer->Release();
    colorBufferView->Release();
    WIN32_CHECK_ERROR(S_OK != swapChainD3D->Present(
        info.presentMode == PresentMode::Immediate ? 0 : 1,
        0
    ));
}
}
