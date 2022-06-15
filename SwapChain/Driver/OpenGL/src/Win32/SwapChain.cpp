#include <GL/Win32/SwapChain.hpp>
#include <GL/Win32/PresentGeometry.hpp>
#include <GL/Win32/PresentShader.hpp>
#include <GL/Win32/PresentTexture.hpp>
#include <GL/Surface.hpp>

#include <GL/Win32/Error.hpp>
#include <GL/Win32/OpenGL.hpp>
#include <GL/Device.hpp>
#include <GL/Image/Format.hpp>
#include <GL/Image/Image.hpp>
#include <GL/Queue/Queue.hpp>
#include <GL/Queue/Semaphore.hpp>
#include <GL/Queue/Fence.hpp>

#include <GL/glew.h>
#include <GL/wglew.h>

#include <iostream>
#include <sstream>

namespace OCRA::SwapChain::Win32
{
void GLAPIENTRY MessageCallback(
    GLenum source,
    GLenum type,
    GLenum id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    if (type == GL_DEBUG_TYPE_ERROR)
    {
        std::stringstream ss{};
        ss << "GL CALLBACK : **GL ERROR * *\n" <<
            " type     = " << type << "\n" <<
            " severity = " << severity << "\n" <<
            " message  = " << message;
        std::cerr << ss.str() << std::endl;
        throw std::runtime_error(ss.str());
    }
}
static inline auto CreateImages(const Device::Handle& a_Device, const Info& a_Info)
{
    std::vector<Image::Handle> images;
    auto win32SwapChain = std::static_pointer_cast<SwapChain::Win32::Impl>(a_Info.oldSwapchain);
    for (auto i = 0u; i < a_Info.imageCount; ++i)
    {
        Image::Handle image;
        if (win32SwapChain != nullptr) {
            const auto& imageInfo = win32SwapChain->images.at(i)->info;
            const bool canRecycle = imageInfo.extent.width == a_Info.imageExtent.width &&
                                    imageInfo.extent.height == a_Info.imageExtent.height &&
                                    imageInfo.arrayLayers == a_Info.imageArrayLayers &&
                                    imageInfo.format == a_Info.imageFormat;
            if (canRecycle) image = win32SwapChain->images.at(i);
        }
        if (image == nullptr) {
            Image::Info imageInfo{};
            imageInfo.type = Image::Type::Image2D;
            imageInfo.extent.width = a_Info.imageExtent.width;
            imageInfo.extent.height = a_Info.imageExtent.height;
            imageInfo.mipLevels = 1;
            imageInfo.arrayLayers = a_Info.imageArrayLayers;
            imageInfo.format = a_Info.imageFormat;
            image = Image::Create(a_Device, imageInfo);
        }
        images.push_back(image);
    }
    return images;
}

Impl::Impl(const Device::Handle& a_Device, const Info& a_Info)
    : SwapChain::Impl(a_Device, a_Info)
    , images(CreateImages(a_Device, a_Info))
{
    const auto pixelSize = GetPixelSize(info.imageFormat);
    if (!WGL_NV_copy_image) {
        std::cerr << "WGL_NV_copy_image unavailable : using slower path\n";
        const auto transferBufferSize = info.imageExtent.width * info.imageExtent.height * pixelSize / 8;
        pixelBuffer.resize(transferBufferSize, 0);
    }
    if (info.oldSwapchain != nullptr && !info.oldSwapchain->retired) {
        auto win32SwapChain = std::static_pointer_cast<SwapChain::Win32::Impl>(info.oldSwapchain);
        win32SwapChain->workerThread.Wait();
        hdc = win32SwapChain->hdc;
        hglrc = win32SwapChain->hglrc;
        presentShader.swap(win32SwapChain->presentShader);
        info.oldSwapchain->Retire();
        info.oldSwapchain.reset();
    }
    else {
        hdc = GetDC(HWND(a_Info.surface->nativeWindow));
        OpenGL::Win32::Initialize();
        const int attribIList[] = {
            WGL_DRAW_TO_WINDOW_ARB, true,
            WGL_SUPPORT_OPENGL_ARB, true,
            WGL_DOUBLE_BUFFER_ARB,  true,
            WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
            WGL_COLORSPACE_EXT,     WGL_COLORSPACE_SRGB_EXT,
            WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
            WGL_COLOR_BITS_ARB,     pixelSize,
            0
        };
        int32_t  pixelFormat = 0;
        uint32_t pixelFormatNbr = 0;
        WIN32_CHECK_ERROR(wglChoosePixelFormatARB(HDC(hdc), attribIList, nullptr, 1, &pixelFormat, &pixelFormatNbr));
        WIN32_CHECK_ERROR(pixelFormat != 0);
        WIN32_CHECK_ERROR(pixelFormatNbr != 0);
        WIN32_CHECK_ERROR(SetPixelFormat(HDC(hdc), pixelFormat, nullptr));
        hglrc = OpenGL::Win32::CreateContext(hdc);
    }
    workerThread.PushCommand([this] {
        WIN32_CHECK_ERROR(wglMakeCurrent(HDC(hdc), HGLRC(hglrc)));
        if (info.presentMode == PresentMode::Immediate) {
            WIN32_CHECK_ERROR(wglSwapIntervalEXT(0));
        }
        else WIN32_CHECK_ERROR(wglSwapIntervalEXT(1));
#ifdef _DEBUG
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(MessageCallback, 0);
#endif _DEBUG
        if (presentTexture == nullptr)
            presentTexture.reset(new PresentTexture(images.front()));
        if (presentShader == nullptr)
            presentShader.reset(new PresentShader);
        if (presentGeometry == nullptr)
            presentGeometry.reset(new PresentGeometry);
        glViewport(0, 0, presentTexture->extent.width, presentTexture->extent.height);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindSampler(0, presentTexture->samplerHandle);
        presentGeometry->Bind();
        presentShader->Bind();
        presentTexture->Bind();
    }, false);
}

Impl::~Impl()
{
    retired = true;
    workerThread.PushCommand([this] {
        presentTexture.reset();
        presentShader.reset();
        presentGeometry.reset();
        wglMakeCurrent(nullptr, nullptr);
    }, true);
    if (hdc != nullptr)
        ReleaseDC(HWND(info.surface->nativeWindow), HDC(hdc));
    if (hglrc != nullptr)
        wglDeleteContext(HGLRC(hglrc));
}

void Impl::Retire() {
    SwapChain::Impl::Retire();
    workerThread.PushCommand([this] {
        presentShader.reset();
        presentTexture.reset();
        presentGeometry.reset();
        wglMakeCurrent(nullptr, nullptr);
    }, true);
    images.clear();
    hdc = nullptr;
    hglrc = nullptr;
}

void Impl::Present(const Queue::Handle& a_Queue)
{
    assert(!retired);
    if (WGL_NV_copy_image) {
        PresentNV(a_Queue);
    }
    else {
        PresentGL(a_Queue);
    }
}

void Impl::PresentNV(const Queue::Handle& a_Queue)
{
    const auto& image = images.at(backBufferIndex);
    const auto extent = image->info.extent;
    workerThread.PushCommand([this, queue = a_Queue, extent] {
        queue->PushCommand([this, extent] {
            auto sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
            glClientWaitSync(sync, 0, 1000);
            glDeleteSync(sync);
            const auto& image = images.at(backBufferIndex);
            WIN32_CHECK_ERROR(wglCopyImageSubDataNV(
                nullptr, image->handle, image->target, //use current context
                0, 0, 0, 0,
                HGLRC(hglrc), presentTexture->handle, presentTexture->target,
                0, 0, 0, 0,
                extent.width, extent.height, 1));
            }, true);
        presentGeometry->Draw();
        WIN32_CHECK_ERROR(SwapBuffers(HDC(hdc)));
        }, info.presentMode != PresentMode::Immediate);
    backBufferIndex = (backBufferIndex + 1) % info.imageCount;
}

void Impl::PresentGL(const Queue::Handle& a_Queue)
{
    const auto& image = images.at(backBufferIndex);
    const auto extent = image->info.extent;
    workerThread.PushCommand([this, queue = a_Queue, extent] {
        queue->PushCommand([this, extent] {
            const auto& image = images.at(backBufferIndex);
            glBindTexture(image->target, image->handle);
            glGetTexImage(
                image->target,
                0,
                image->dataFormat,
                image->dataType,
                pixelBuffer.data());
            glBindTexture(image->target, 0);
            }, true);
        glTexSubImage2D(
            presentTexture->target, 0,
            0, 0, extent.width, extent.height,
            presentTexture->dataFormat, presentTexture->dataType, pixelBuffer.data());
        presentGeometry->Draw();
        WIN32_CHECK_ERROR(SwapBuffers(HDC(hdc)));
        }, info.presentMode != PresentMode::Immediate);
    backBufferIndex = (backBufferIndex + 1) % info.imageCount;
}

//TODO: implement a timeout
Image::Handle Impl::AcquireNextImage(
    const std::chrono::nanoseconds& a_Timeout,
    const Queue::Semaphore::Handle& a_Semaphore,
    const Queue::Fence::Handle& a_Fence)
{
    workerThread.Wait();
    //We do not need to synchronize with the GPU for real here
    if (a_Semaphore != nullptr) {
        if (a_Semaphore->type == Queue::Semaphore::Type::Binary)
            std::static_pointer_cast<Queue::Semaphore::Binary>(a_Semaphore)->SignalNoSync();
        else throw std::runtime_error("Cannot wait on Timeline Semaphores when presenting");
    }
    if (a_Fence != nullptr) a_Fence->SignalNoSync();
    return images.at(backBufferIndex);
}
}
