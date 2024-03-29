#include <OCRA/OCRA.hpp>
#include <OCRA/SwapChain/Core.hpp>

#include <GL/Win32/Error.hpp>
#include <GL/Win32/OpenGL.hpp>
#include <GL/Win32/PhysicalDevice.hpp>
#include <GL/Win32/PresentGeometry.hpp>
#include <GL/Win32/PresentPixels.hpp>
#include <GL/Win32/PresentShader.hpp>
#include <GL/Win32/PresentTexture.hpp>
#include <GL/Win32/Surface.hpp>
#include <GL/Win32/SwapChain.hpp>

#include <GL/Common/Assert.hpp>
#include <GL/Common/BufferOffset.hpp>
#include <GL/Device.hpp>
#include <GL/Enums.hpp>
#include <GL/Fence.hpp>
#include <GL/Image.hpp>
#include <GL/Queue.hpp>
#include <GL/Semaphore.hpp>

#include <GL/glew.h>
#include <GL/wglew.h>

#include <iostream>
#include <sstream>

namespace OCRA::SwapChain {
void GLAPIENTRY MessageCallback(
    GLenum source,
    GLenum type,
    GLenum id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    if (type == GL_DEBUG_TYPE_ERROR) {
        std::stringstream ss {};
        ss << "GL CALLBACK : **GL ERROR * *\n"
           << " type     = " << type << "\n"
           << " severity = " << severity << "\n"
           << " message  = " << message;
        std::cerr << ss.str() << std::endl;
        throw std::runtime_error(ss.str());
    }
}
static inline auto CreateImages(const Device::Handle& a_Device, const CreateSwapChainInfo& a_Info)
{
    std::vector<Image::Handle> images;
    images.reserve(a_Info.imageCount);
    for (auto i = 0u; i < a_Info.imageCount; ++i) {
        Image::Handle image;
        if (a_Info.oldSwapchain != nullptr && a_Info.oldSwapchain->images.size() > i) {
            const auto& imageInfo = a_Info.oldSwapchain->images.at(i)->info;
            const bool canRecycle = imageInfo.extent.width == a_Info.imageExtent.width && imageInfo.extent.height == a_Info.imageExtent.height && imageInfo.arrayLayers == a_Info.imageArrayLayers && imageInfo.format == a_Info.imageFormat;
            if (canRecycle)
                image = a_Info.oldSwapchain->images.at(i);
        }
        if (image == nullptr) {
            CreateImageInfo imageInfo {};
            imageInfo.type          = ImageType::Image2D;
            imageInfo.extent.width  = a_Info.imageExtent.width;
            imageInfo.extent.height = a_Info.imageExtent.height;
            imageInfo.extent.depth  = 1;
            imageInfo.mipLevels     = 1;
            imageInfo.arrayLayers   = a_Info.imageArrayLayers;
            imageInfo.format        = a_Info.imageFormat;
            image                   = CreateImage(a_Device, imageInfo);
        }
        images.push_back(image);
    }
    return images;
}

Impl::Impl(const Device::Handle& a_Device, const CreateSwapChainInfo& a_Info)
    : device(a_Device)
    , surface(a_Info.surface)
    , presentMode(a_Info.presentMode)
    , images(CreateImages(a_Device, a_Info))
{
    const auto pixelSize = GetPixelSize(a_Info.imageFormat);
    auto win32SwapChain  = a_Info.oldSwapchain;
    if (win32SwapChain != nullptr && !win32SwapChain->retired) {
        win32SwapChain->workerThread.Wait();
        hdc   = win32SwapChain->hdc;
        hglrc = win32SwapChain->hglrc;
        presentShader.swap(win32SwapChain->presentShader);
        win32SwapChain->Retire();
        win32SwapChain.reset();
    } else {
        hdc = GetDC(HWND(a_Info.surface->hwnd));
        OpenGL::Win32::Initialize();
        const int attribIList[] = {
            WGL_DRAW_TO_WINDOW_ARB, true,
            WGL_SUPPORT_OPENGL_ARB, true,
            WGL_DOUBLE_BUFFER_ARB, true,
            WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
            WGL_COLORSPACE_EXT, WGL_COLORSPACE_SRGB_EXT,
            WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
            WGL_COLOR_BITS_ARB, int(pixelSize),
            0
        };
        int32_t pixelFormat     = 0;
        uint32_t pixelFormatNbr = 0;
        WIN32_CHECK_ERROR(wglChoosePixelFormatARB(HDC(hdc), attribIList, nullptr, 1, &pixelFormat, &pixelFormatNbr));
        WIN32_CHECK_ERROR(pixelFormat != 0);
        WIN32_CHECK_ERROR(pixelFormatNbr != 0);
        WIN32_CHECK_ERROR(SetPixelFormat(HDC(hdc), pixelFormat, nullptr));
        hglrc = OpenGL::Win32::CreateContext(hdc);
    }
    workerThread.PushCommand([this, &a_Info, pixelSize] {
        WIN32_CHECK_ERROR(wglMakeCurrent(HDC(hdc), HGLRC(hglrc)));
        if (a_Info.presentMode == SwapChainPresentMode::Immediate) {
            WIN32_CHECK_ERROR(wglSwapIntervalEXT(0));
        } else
            WIN32_CHECK_ERROR(wglSwapIntervalEXT(1));
#ifdef _DEBUG
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(MessageCallback, 0);
#endif _DEBUG
        if (presentTexture == nullptr) {
            presentTexture.reset(new Win32::PresentTexture(images.front()));
            presentTexture->Bind();
        }
        if (presentShader == nullptr) {
            presentShader.reset(new Win32::PresentShader);
            presentShader->Bind();
        }
        if (presentGeometry == nullptr) {
            presentGeometry.reset(new Win32::PresentGeometry);
            presentGeometry->Bind();
        }
        if (!WGLEW_NV_copy_image && presentPixels == nullptr) {
            static bool warningPrinted = false;
            if (!warningPrinted) {
                std::cerr << "SwapChain : WGL_NV_copy_image unavailable, using slower path\n";
                warningPrinted = true;
            }
            const auto transferBufferSize = a_Info.imageExtent.width * a_Info.imageExtent.height * pixelSize / 8;
            presentPixels.reset(new Win32::PresentPixels(transferBufferSize));
            presentPixels->Bind();
        }
        glViewport(0, 0, presentTexture->extent.width, presentTexture->extent.height);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindSampler(0, presentTexture->samplerHandle);
    },
        true);
    OCRA_ASSERT(surface != nullptr);
}

Impl::~Impl()
{
    retired = true;
    workerThread.PushCommand([this] {
        presentTexture.reset();
        presentShader.reset();
        presentGeometry.reset();
        presentPixels.reset();
        wglMakeCurrent(nullptr, nullptr);
    },
        true);
    if (hdc != nullptr)
        ReleaseDC(HWND(surface->hwnd), HDC(hdc));
    if (hglrc != nullptr)
        wglDeleteContext(HGLRC(hglrc));
}

void Impl::Retire()
{
    workerThread.PushCommand([this] {
        presentShader.reset();
        presentTexture.reset();
        presentGeometry.reset();
        presentPixels.reset();
        wglMakeCurrent(nullptr, nullptr);
    },
        true);
    images.clear();
    hdc     = nullptr;
    hglrc   = nullptr;
    retired = true;
}

void Impl::Present(const Queue::Handle& a_Queue)
{
    OCRA_ASSERT(!retired);
    if (WGLEW_NV_copy_image) {
        PresentNV(a_Queue);
    } else {
        PresentGL(a_Queue);
    }
}

void Impl::PresentNV(const Queue::Handle& a_Queue)
{
    // Only Mailbox mode can stack several presentation requests
    if (presentMode != SwapChainPresentMode::Mailbox)
        workerThread.Wait();
    workerThread.PushCommand([this, queue = a_Queue] {
        queue->PushCommand([this] {
            const auto& image  = images.at(backBufferIndex);
            const auto& extent = image->info.extent;
            WIN32_CHECK_ERROR(wglCopyImageSubDataNV(
                nullptr, image->handle, image->target, // use current context
                0, 0, 0, 0,
                HGLRC(hglrc), presentTexture->handle, presentTexture->target,
                0, 0, 0, 0,
                extent.width, extent.height, 1));
        },
            true);
        presentGeometry->Draw();
        WIN32_CHECK_ERROR(SwapBuffers(HDC(hdc)));
    },
        false);
    backBufferIndex = (backBufferIndex + 1) % images.size();
}

void Impl::PresentGL(const Queue::Handle& a_Queue)
{
    // Only Mailbox mode can stack several presentation requests
    if (presentMode != SwapChainPresentMode::Mailbox)
        workerThread.Wait();
    workerThread.PushCommand([this, queue = a_Queue] {
        queue->PushCommand([this] {
            const auto& image = images.at(backBufferIndex);
            glBindTexture(image->target, image->handle);
            glGetTexImage(
                image->target,
                0,
                image->dataFormat,
                image->dataType,
                presentPixels->GetPtr());
            glBindTexture(image->target, 0);
        },
            true);
        const auto& image  = images.at(backBufferIndex);
        const auto& extent = image->info.extent;
        presentPixels->Flush();
        glTexSubImage2D(
            presentTexture->target, 0,
            0, 0, extent.width, extent.height,
            presentTexture->dataFormat, presentTexture->dataType, BUFFER_OFFSET(presentPixels->offset));
        presentGeometry->Draw();
        WIN32_CHECK_ERROR(SwapBuffers(HDC(hdc)));
    },
        false);
    backBufferIndex = (backBufferIndex + 1) % images.size();
}
}

namespace OCRA::Device {
SwapChain::Handle CreateSwapChain(const Device::Handle& a_Device, const CreateSwapChainInfo& a_Info)
{
    return std::make_shared<SwapChain::Impl>(a_Device, a_Info);
}
}

namespace OCRA::SwapChain {
uint32_t GetImageCount(const Handle& a_SwapChain)
{
    return a_SwapChain->images.size();
}

std::pair<Image::Handle, uint32_t> GetNextImage(
    const Handle& a_SwapChain,
    const std::chrono::nanoseconds& a_Timeout,
    const Semaphore::Handle& a_Semaphore,
    const Fence::Handle& a_Fence)
{
    a_SwapChain->device.lock()->PushCommand([semaphore = a_Semaphore, fence = a_Fence] {
        if (semaphore != nullptr) {
            OCRA_ASSERT(semaphore->type == SemaphoreType::Binary && "Cannot wait on Timeline Semaphores when requesting next image");
            std::static_pointer_cast<Semaphore::Binary>(semaphore)->Signal();
        }
        if (fence != nullptr)
            fence->Signal();
    },
        false);
    return { a_SwapChain->images.at(a_SwapChain->backBufferIndex), a_SwapChain->backBufferIndex };
}
}

namespace OCRA::Queue {
void Present(const Queue::Handle& a_Queue, const SwapChainPresentInfo& a_PresentInfo)
{
    a_Queue->PushCommand([a_PresentInfo] {
        for (const auto& semaphore : a_PresentInfo.waitSemaphores) {
            OCRA_ASSERT(semaphore->type == SemaphoreType::Binary && "Cannot wait on Timeline Semaphores when presenting");
            std::static_pointer_cast<Semaphore::Binary>(semaphore)->Wait();
        }
    },
        false);
    for (const auto& swapChain : a_PresentInfo.swapChains)
        swapChain->Present(a_Queue);
}
}
