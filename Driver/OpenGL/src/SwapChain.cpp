#include <SwapChain.hpp>
#include <Image/Image.hpp>

#include <GL/Common/DefaultPixelFormat.hpp>
#include <GL/Common/Error.hpp>
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
        imageInfo.mipLevels = 1;
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
            //assert(info.oldSwapchain->info.imageExtent == info.imageExtent);
            assert(info.oldSwapchain->info.imageSharingMode == info.imageSharingMode);
            frameBufferHandle = info.oldSwapchain->frameBufferHandle;
            if (info.oldSwapchain->info.imageExtent.width >= info.imageExtent.width &&
                info.oldSwapchain->info.imageExtent.height >= info.imageExtent.height) {
                info.imageExtent = info.oldSwapchain->info.imageExtent;
                images = info.oldSwapchain->images;
            }
            else images = CreateImages(a_Device, a_Info);
            info.oldSwapchain->Retire();
            info.oldSwapchain.reset();
            return;
        }
        a_Device->PushCommand(0, 0, [this] {
            glGenFramebuffers(1, &frameBufferHandle);
        }, true);
        Win32::SetDefaultPixelFormat(std::static_pointer_cast<Surface::Win32::Impl>(info.surface)->hdc);
        images = CreateImages(a_Device, a_Info);
    }
    void Retire() {
        retired = true;
    }
    void Present(const Queue::Handle& a_Queue, const uint32_t& a_ImageIndex) {
        assert(!retired);
        const auto deviceHandle = device.lock();
        const auto physicalDevice = deviceHandle->physicalDevice.lock();
        const auto hdc = HDC(std::static_pointer_cast<Surface::Win32::Impl>(info.surface)->hdc);
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
                windowWidth,
                windowHeight,
                0,
                0,
                windowWidth,
                windowHeight,
                GL_COLOR_BUFFER_BIT,
                GL_NEAREST);
            glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
            //if (!wglSwapLayerBuffers(hdc, WGL_SWAP_MAIN_PLANE))
            WIN32_CHECK_ERROR(SwapBuffers(hdc));
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