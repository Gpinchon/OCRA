#include <GL/Win32/SwapChain.hpp>
#include <GL/Win32/Surface.hpp>
#include <GL/Win32/Error.hpp>

#include <GL/glew.h>
#include <GL/wglew.h>

#include <dxgi1_3.h>
#include <d3d11.h>
#include <comdef.h>


namespace OCRA::SwapChain::Win32
{
Impl::Impl(const Device::Handle& a_Device, const Info& a_Info)
    : SwapChain::Impl(a_Device, a_Info)
{
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //TODO give the real Image Format
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
        D3D11_SDk_VERSION,        //SDK Version
        &swapChainDesc,           //SwapChain Description
        &swapChain,               //SwapChain PTR
        &device,                  //Device PTR
        nullptr,                  //Feature Level
        &deviceContext            //Immediat Context
    );
    glHandleD3D = wglDxOpenDeviceNV(device);
    WIN32_CHECK_ERROR(glHandleD3D != nullptr);
    a_Device->PushCommand([this] {
        glGenTextures(1, &renderTextureHandle);
    }, true);
}
void Present(const Queue::Handle& a_Queue, const uint32_t& a_ImageIndex)
{
    assert(!retired);
    const auto surfaceRect = info.surface->GetRect();
    const auto deviceD3D        = (ID3D11Device*)device;
    const auto deviceContextD3D = (ID3D11DeviceContext*)deviceContext;
    const auto swapChainD3D     = (IDXGISwapChain*)swapChain;
    ID3D11Texture2D*        colorBuffer;
    ID3D11RenderTargetView* colorBufferView;
    GLuint  colorBufferNameGL;
    swapChainD3D->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&colorBuffer);
    deviceD3D->CreateRenderTargetView(
        colorBuffer,
        &CD3D11_RENDER_TARGET_VIEW_DESC(D3D11_RTV_DIMENSION_TEXTURE2D, DXGI_FORMAT_R8G8B8A8_UNORM),
        &colorBufferView
    );

    const auto colorBufferHandle = wglDXRegisterObjectNV(glHandleD3D, colorBuffer, renderTextureHandle, GL_TEXTURE_2D, WGL_ACCESS_READ_WRITE_NV);
    WIN32_CHECK_ERROR(colorBufferHandle != nullptr);
    deviceContextD3D->OMSetRenderTargets(1, &colorBufferView, nullptr);
    wglDXLockObjectsNV(glHandleD3D, 1, &colorBufferHandle);

    a_Queue->PushCommand([this, imageIndex = a_ImageIndex, surfaceRect] {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBufferHandle);
        glFramebufferTexture2D(
            GL_READ_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D,
            images.at(imageIndex)->handle,
            0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glFramebufferTexture2D(
            GL_READ_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D,
            renderTextureHandle,
            0);
        glBlitFramebuffer(
            0,
            0,
            surfaceRect.width,
            surfaceRect.height,
            0,
            0,
            surfaceRect.width,
            surfaceRect.height,
            GL_COLOR_BUFFER_BIT,
            GL_NEAREST);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    }, true);

    wglDXUnlockObjectsNV(glHandleD3D, 1, &colorBufferHandle);
    WIN32_CHECK_ERROR(swapChainD3D->Present(0, 0));
    wglUnregisterObjectNV(glHandleD3D, colorBufferHandle);
    colorBuffer->Release();
    colorBufferView->Release();
}
}
