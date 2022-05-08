#include <Common.hpp>

#include <Instance.hpp>
#include <Device.hpp>
#include <Surface.hpp>
#include <SwapChain.hpp>
#include <Image/Image.hpp>
#include <Image/View.hpp>
#include <RenderPass.hpp>
#include <Command/Pool.hpp>
#include <Command/Buffer.hpp>
#include <Command/RenderPass.hpp>
#include <Command/Scissor.hpp>
#include <Command/ViewPort.hpp>
#include <Queue/Fence.hpp>

#include <Windows.h>

using namespace OCRA;

#include <Pipeline/Graphics.hpp>

static inline auto CreateGraphicsPipeline(const Device::Handle& a_Device, const RenderPass::Handle& a_RenderPass, const uint32_t a_Width, const uint32_t a_Height)
{
    Pipeline::Graphics::Info graphicsPipelineInfo;
    ViewPort viewport;
    viewport.rect.offset = { 0, 0 };
    viewport.rect.extent = { a_Width, a_Height };
    viewport.depthRange  = { 0, 1 };
    Rect2D  scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = { a_Width, a_Height };
    graphicsPipelineInfo.renderPass = a_RenderPass;
    graphicsPipelineInfo.inputAssemblyState.primitiveRestartEnable = false;
    graphicsPipelineInfo.viewPortState.viewPorts = { viewport };
    graphicsPipelineInfo.viewPortState.scissors  = { scissor };
    graphicsPipelineInfo.dynamicState.dynamicStates = { Pipeline::DynamicState::State::Viewport, Pipeline::DynamicState::State::Scissor };
    //Everything else is left by default for now
    return Pipeline::Graphics::Create(a_Device, graphicsPipelineInfo);
}

static inline auto CreateRenderPass(const Device::Handle& a_Device)
{
    RenderPass::Info renderPassInfo{};
    //TODO fill this
    return RenderPass::Create(a_Device, renderPassInfo);
}

static inline auto CreateFrameBuffer(const Device::Handle& a_Device, const std::vector<Image::Handle> a_Attachments)
{
    FrameBuffer::Info frameBufferInfo{};
    for (const auto& image : a_Attachments) {
        Image::View::Info imageViewInfo;
        imageViewInfo.image = image;
        imageViewInfo.format = Image::Format::Uint8_Normalized_RGBA;
        imageViewInfo.type = Image::View::Type::View2D;
        const auto imageView = Image::View::Create(a_Device, imageViewInfo);
        frameBufferInfo.attachments.push_back(imageView);
    }
    frameBufferInfo.extent.depth = 1;
    return FrameBuffer::Create(a_Device, frameBufferInfo);
}

static inline void RecordMainCommandBuffer(
    const Command::Buffer::Handle& a_MainCommandBuffer,
    const FrameBuffer::Handle&     a_FrameBuffer,
    const Pipeline::Handle&        a_GraphicsPipeline,
    const RenderPass::Handle&      a_RenderPass,
    const uint32_t a_Width, const uint32_t a_Height)
{
    Command::Buffer::BeginInfo bufferBeginInfo{};
    bufferBeginInfo.flags = Command::Buffer::UsageFlagBits::None;
    ViewPort viewport;
    viewport.rect.offset = { 0, 0 };
    viewport.rect.extent = { a_Width, a_Height };
    viewport.depthRange  = { 0, 1 };
    Rect2D  scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = { a_Width, a_Height };
    Command::RenderPassBeginInfo renderPassBeginInfo{};
    Command::SubPassContents     subPassContents{};
    renderPassBeginInfo.renderPass = a_RenderPass;
    renderPassBeginInfo.framebuffer = a_FrameBuffer;
    renderPassBeginInfo.renderArea.offset = { 0, 0 };
    renderPassBeginInfo.renderArea.extent = { a_Width, a_Height };
    renderPassBeginInfo.clearValues = { {0, 0, 0, 0} };

    Command::Buffer::Reset(a_MainCommandBuffer);
    Command::Buffer::Begin(a_MainCommandBuffer, bufferBeginInfo);
    {
        Command::BeginRenderPass(a_MainCommandBuffer, renderPassBeginInfo, subPassContents);
        {
            Command::SetScissor(a_MainCommandBuffer, 0, { scissor });
            Command::SetViewPort(a_MainCommandBuffer, 0, { viewport });
            Command::BindPipeline(a_MainCommandBuffer, Pipeline::BindingPoint::Graphics, a_GraphicsPipeline);
            //draw stuff here
        }
        Command::EndRenderPass(a_MainCommandBuffer);
    }
    Command::Buffer::End(a_MainCommandBuffer);
}

int GraphicsPipeline()
{
    int ret = 0;
    auto window = Window("Test_SwapChain", 1280, 720);
    //basic setup as usual
    const auto instance = CreateInstance("Test_SwapChain");
    const auto surface = CreateSurface(instance, GetModuleHandle(0), (void*)window.nativeHandle);
    const auto physicalDevice = Instance::EnumeratePhysicalDevices(instance).front();
    const auto device = CreateDevice(physicalDevice);
    const auto queueFamily = FindQueueFamily(physicalDevice, PhysicalDevice::QueueFlagsBits::Transfer);
    const auto queue = Device::GetQueue(device, queueFamily, 0); //Get first available queue
    const auto commandPool = CreateCommandPool(device, queueFamily);
    const auto renderPass = CreateRenderPass(device);
    const auto imageAcquisitionFence = Queue::Fence::Create(device);

    Command::Buffer::Handle mainCommandBuffer  = CreateCommandBuffer(device, commandPool, Command::Pool::AllocateInfo::Level::Primary);

    FrameBuffer::Handle     frameBuffer;
    Pipeline::Handle        graphicsPipeline;
    SwapChain::Handle       swapChain;
    SwapChain::PresentInfo  presentInfo;
    uint32_t width = 0, height = 0;
    bool close = false;

    window.OnResize = [&](const Window&, const uint32_t a_Width, const uint32_t a_Height) {
        swapChain = CreateSwapChain(device, surface, swapChain, a_Width, a_Height, 2);
        graphicsPipeline = CreateGraphicsPipeline(device, renderPass, a_Width, a_Height);
        presentInfo.swapChains = { swapChain };
        width = a_Width;
        height = a_Height;
    };
    window.OnClose = [&close](const Window&) {
        close = true;
    };
    window.Show();
    while (true) {
        window.PushEvents();
        if (close) break;
        const auto nextImage = SwapChain::AcquireNextImage(device, swapChain, std::chrono::nanoseconds(15000000), nullptr, imageAcquisitionFence);
        Queue::Fence::WaitFor(device, imageAcquisitionFence, std::chrono::nanoseconds(15000000));
        Queue::Fence::Reset(device, { imageAcquisitionFence });
        presentInfo.imageIndices = { nextImage };
        const auto swapChainImage = SwapChain::GetImages(device, swapChain).at(nextImage);
        const auto frameBuffer = CreateFrameBuffer(device, { swapChainImage });
        RecordMainCommandBuffer(mainCommandBuffer, frameBuffer, graphicsPipeline, renderPass, width, height);
        SubmitCommandBuffer(queue, mainCommandBuffer);
        SwapChain::Present(queue, presentInfo);
        Queue::WaitIdle(queue);
    }
    return ret;
}
