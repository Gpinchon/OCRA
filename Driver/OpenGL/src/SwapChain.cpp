#include <SwapChain.hpp>
#include <Image/Image.hpp>

#include <GL/Queue/Semaphore.hpp>
#include <GL/Queue/Queue.hpp>
#include <GL/PhysicalDevice.hpp>
#include <GL/SwapChain.hpp>
#include <GL/Surface.hpp>
#include <GL/Device.hpp>
#include <GL/Image/Image.hpp>
#include <GL/Image/Format.hpp>
#include <GL/WeakHandle.hpp>
#include <GL/glew.h>

#include <cassert>
#include <stdexcept>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

//TODO : use WGL_NV_DX_interop & WGL_NV_DX_interop2
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

static inline auto CanRecycleOldSwapChainImages(
    const Info& a_OldInfo, const Extent2D& a_OldExtent,
    const Info& a_Info)
{
    return a_OldExtent.width          >= a_Info.imageExtent.width
        && a_OldExtent.height         >= a_Info.imageExtent.height
        && a_OldInfo.minImageCount    >= a_Info.minImageCount
        && a_OldInfo.imageFormat      == a_Info.imageFormat
        && a_OldInfo.imageColorSpace  == a_Info.imageColorSpace
        && a_OldInfo.imageSharingMode == a_Info.imageSharingMode;
}

Impl::Impl(const Device::Handle& a_Device, const Info& a_Info)
    : info(a_Info)
    , device(a_Device)
    , realExtent(info.imageExtent)
{
    if (info.oldSwapchain != nullptr) {
        frameBufferHandle = info.oldSwapchain->frameBufferHandle;
        if (CanRecycleOldSwapChainImages(info.oldSwapchain->info, info.oldSwapchain->realExtent, info)) {
            realExtent = info.oldSwapchain->realExtent;
            images = info.oldSwapchain->images;
        }
        else images = CreateImages(a_Device, a_Info);
        info.oldSwapchain->Retire();
        info.oldSwapchain.reset();
        return;
    }
    a_Device->PushCommand([this] {
        glGenFramebuffers(1, &frameBufferHandle);
    }, true);
    images = CreateImages(a_Device, a_Info);
}

Impl::~Impl() {
    if (frameBufferHandle != 0)
        device.lock()->PushCommand([frameBufferHandle = frameBufferHandle] {
            glDeleteFramebuffers(1, &frameBufferHandle);
        }, false);
}

void Impl::Present(const Queue::Handle& a_Queue, const uint32_t& a_ImageIndex)
{
    assert(!retired);
    const auto physicalDevice = device.lock()->physicalDevice.lock();
    const auto surfaceRect = info.surface->GetRect();
    physicalDevice->SetSurface(info.surface);
    a_Queue->PushCommand([this, imageIndex = a_ImageIndex, surfaceRect] {
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
            surfaceRect.width,
            surfaceRect.height,
            0,
            0,
            surfaceRect.width,
            surfaceRect.height,
            GL_COLOR_BUFFER_BIT,
            GL_NEAREST);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    }, false);
    physicalDevice->SwapBuffers(info.presentMode);
}

Handle Create(const Device::Handle& a_Device, const Info& a_Info)
{
    return Handle(new Impl(a_Device, a_Info));
}
const std::vector<Image::Handle>& GetImages(const Device::Handle& a_Device, const Handle& a_SwapChain)
{
    return a_SwapChain->images;
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