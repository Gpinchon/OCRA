#include <SwapChain.hpp>
#include <Image/Image.hpp>

#include <GL/Queue/Semaphore.hpp>
#include <GL/Queue/Queue.hpp>
#include <GL/PhysicalDevice.hpp>
#include <GL/Surface.hpp>
#include <GL/Device.hpp>
#include <GL/Image/Image.hpp>
#include <GL/Image/Format.hpp>
#include <GL/WeakHandle.hpp>
#include <GL/wglew.h>

#include <stdexcept>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::SwapChain
{
bool operator==(const Extent2D& a, const Extent2D& b) {
    return a.height == b.height
        && a.width == b.width;
}
static inline auto CreateImages(const Device::Handle& a_Device, const Info& a_Info)
{
    std::vector<Image::Handle> images;
    for (auto i = 0u; i < a_Info.minImageCount; ++i)
    {
        Image::Info imageInfo{};
        imageInfo.type = Image::Type::Image2D;
        imageInfo.extent.width = a_Info.imageExtent.width;
        imageInfo.extent.height = a_Info.imageExtent.height;
        imageInfo.arrayLayers = a_Info.imageArrayLayers;
        imageInfo.format = a_Info.imageFormat;
        images.push_back(Image::Create(a_Device, imageInfo));
    }
    return images;
}
constexpr auto FORMATSMAX = 32;
struct Impl
{
    Impl(const Device::Handle& a_Device, const Info& a_Info)
        : info(a_Info)
        , device(a_Device)
    {
        if (info.oldSwapchain != nullptr) {
            assert(info.oldSwapchain->info.imageFormat == info.imageFormat);
            assert(info.oldSwapchain->info.imageColorSpace == info.imageColorSpace);
            assert(info.oldSwapchain->info.imageExtent == info.imageExtent);
            assert(info.oldSwapchain->info.imageSharingMode == info.imageSharingMode);
            frameBufferHandle = info.oldSwapchain->frameBufferHandle;
            if (info.oldSwapchain->info.imageExtent.width >= info.imageExtent.width &&
                info.oldSwapchain->info.imageExtent.height >= info.imageExtent.height)
                images = info.oldSwapchain->images;
            else images = CreateImages(a_Device, a_Info);
            info.oldSwapchain->Retire();
            info.oldSwapchain.reset();
            return;
        }
        a_Device->PushCommand(0, 0, [this] {
            glGenFramebuffers(1, &frameBufferHandle);
        }, true);
        const auto hdc = GetDC(HWND(info.surface->nativeWindow));
        const auto rBits = GetRedSize(info.imageFormat);
        const auto gBits = GetGreenSize(info.imageFormat);
        const auto bBits = GetBlueSize(info.imageFormat);
        const auto aBits = GetAlphaSize(info.imageFormat);
        const auto dBits = GetDepthSize(info.imageFormat);
        const auto sBits = GetStencilSize(info.imageFormat);
        const auto glDataType = GetGLDataType(info.imageFormat);
        const auto fRGBA = glDataType == GL_HALF_FLOAT || glDataType == GL_FLOAT;
        const auto sRGB = info.imageColorSpace == Image::ColorSpace::sRGB;
        const int attribIList[] = {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
            WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
            WGL_COLORSPACE_EXT,     sRGB ? WGL_COLORSPACE_SRGB_EXT : WGL_COLORSPACE_LINEAR_EXT,
            WGL_PIXEL_TYPE_ARB,     fRGBA ? WGL_TYPE_RGBA_FLOAT_ARB : WGL_TYPE_RGBA_ARB,
            WGL_COLOR_BITS_ARB,     rBits + gBits + bBits + aBits,
            WGL_RED_BITS_EXT,       rBits,
            WGL_GREEN_BITS_EXT,     gBits,
            WGL_BLUE_BITS_EXT,      bBits,
            WGL_ALPHA_BITS_EXT,     aBits,
            WGL_DEPTH_BITS_EXT,     dBits,
            WGL_STENCIL_BITS_EXT,   sBits,
            0
        };
        const float attribFList[] = {
            0
        };
        int32_t  wglFormats[FORMATSMAX];
        uint32_t wglFormatsNbr = 0;
        wglChoosePixelFormatARB(hdc,
            attribIList,
            attribFList,
            FORMATSMAX,
            wglFormats,
            &wglFormatsNbr);
        if (wglFormatsNbr == 0) throw std::runtime_error("Could not find pixel format");
        PIXELFORMATDESCRIPTOR pfd[FORMATSMAX]{};
        for (auto index = 0u; index < wglFormatsNbr; ++index)
            DescribePixelFormat(hdc, wglFormats[index], sizeof(PIXELFORMATDESCRIPTOR), &pfd[index]);
        if (!SetPixelFormat(hdc, wglFormats[0], &pfd[0]))
            throw std::runtime_error("Failed to set Pixel Format");
        images = CreateImages(a_Device, a_Info);
    }
    void Retire() {
        retired = true;
    }
    void Present(const Queue::Handle& a_Queue, const uint32_t& a_ImageIndex) {
        assert(!retired);
        const auto deviceHandle = device.lock();
        const auto physicalDevice = deviceHandle->physicalDevice.lock();
        const auto hdc = GetDC(HWND(info.surface->nativeWindow));
        physicalDevice->SetDeviceHandle(hdc);
        physicalDevice->SetSwapInterval(uint8_t(info.presentMode));
        device.lock()->PushCommand(a_Queue->familyIndex, a_Queue->queueIndex, [this, imageIndex = a_ImageIndex, hdc] {
            RECT windowRect;
            GetWindowRect(
                HWND(info.surface->nativeWindow),
                &windowRect);
            const auto windowWidth = windowRect.right - windowRect.left;
            const auto windowHeight = windowRect.bottom - windowRect.top;
            glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBufferHandle);
            glFramebufferTexture2D(
                GL_READ_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_2D,
                images.at(imageIndex)->handle,
                0);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            glBlitFramebuffer(
                0,
                0,
                info.imageExtent.width,
                info.imageExtent.height,
                0,
                0,
                windowWidth,
                windowHeight,
                GL_COLOR_BUFFER_BIT,
                GL_NEAREST);
            glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
            SwapBuffers(hdc);
        }, true);
    }
    Info                        info;
    const Device::WeakHandle    device;
    bool                        retired{ false };
    std::vector<Image::Handle>  images;
    uint32_t                    frameBufferHandle;
};
Handle Create(const Device::Handle& a_Device, const Info& a_Info)
{
    return Handle(new Impl(a_Device, a_Info));
}
std::vector<Image::Handle> GetImages(const Device::Handle& a_Device, const Handle& a_SwapChain)
{
    return { a_SwapChain->images };
}
void Present(const Queue::Handle& a_Queue, const PresentInfo& a_PresentInfo)
{
    for (const auto& semaphore : a_PresentInfo.waitSemaphores)
    {
        if (semaphore->type == Queue::Semaphore::Type::Binary)
            std::static_pointer_cast<Queue::Semaphore::Binary>(semaphore)->Wait();
        else throw std::runtime_error("Cannot wait on Timeline Semaphores when presenting");
    }
    for (auto index = 0u; index < a_PresentInfo.swapChains.size(); ++index) {
        const auto& swapChain = a_PresentInfo.swapChains.at(index);
        const auto& swapChainImage = a_PresentInfo.imageIndices.at(index);
        swapChain->Present(a_Queue, swapChainImage);
    }
}
}