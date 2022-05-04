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

#include <Windows.h>

using namespace OCRA;

#define SWAPCHAIN_IMAGE_NBR 2

#include <Pipeline/Graphics.hpp>

static inline auto CreateGraphicsPipeline(const Device::Handle& a_Device, const RenderPass::Handle& a_RenderPass, const uint32_t a_Width, const uint32_t a_Height)
{
    Pipeline::Graphics::Info graphicsPipelineInfo;
    ViewPort viewport;
    viewport.rect.offset = { 0, 0 };
    viewport.rect.extent = { a_Width, a_Height };
    viewport.depthRange  = { 0, 1 };
    iRect2D  scissor;
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
    RenderPass::Create(a_Device, renderPassInfo);
}

static inline auto CreateFrameBuffer(const Device::Handle& a_Device, const std::vector<Image::Handle> a_Attachments)
{
    FrameBuffer::Info frameBufferInfo{};
    for (const auto& image : a_Attachments) {
        Image::View::Info imageViewInfo;
        imageViewInfo.image = image;
        const auto imageView = Image::View::Create(a_Device, imageViewInfo);
        frameBufferInfo.attachments.push_back(imageView);
    }
    frameBufferInfo.extent.depth = 1;
    return FrameBuffer::Create(a_Device, frameBufferInfo);
}

static inline void RecordMainCommandBuffer(
    Command::Buffer::Handle&    a_MainCommandBuffer,
    FrameBuffer::Handle&        a_FrameBuffer,
    Pipeline::Handle&           a_GraphicsPipeline,
    RenderPass::Handle&         a_RenderPass,
    const uint32_t a_Width, const uint32_t a_Height)
{
    Command::Buffer::BeginInfo commandBufferBeginInfo{};
    bufferBeginInfo.flags = Command::Buffer::UsageFlagBits::None;
    ViewPort viewport;
    viewport.rect.offset = { 0, 0 };
    viewport.rect.extent = { a_Width, a_Height };
    viewport.depthRange  = { 0, 1 };
    iRect2D  scissor;
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
    Command::Buffer::Begin(a_MainCommandBuffer, commandBufferBeginInfo);
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

int SwapChain()
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

    Command::Buffer::Handle mainCommandBuffer  = CreateCommandBuffer(device, commandPool, Command::Pool::AllocateInfo::Level::Primary);

    std::vector<FrameBuffer::Handle> frameBuffers;
    Pipeline::Handle        graphicsPipeline;
    SwapChain::Handle       swapChain;
    SwapChain::PresentInfo  presentInfo;
    uint32_t frameIndex = 0;
    uint32_t lastSwapChainImageIndex = 0;
    bool first = true;
    bool close = false;

    window.OnResize = [&device, &renderPass, &swapChain, &frameBuffers, &presentInfo](const Window&, const uint32_t a_Width, const uint32_t a_Height) {
        frameBuffers.clear();
        swapChain = CreateSwapChain(device, surface, swapChain, a_Width, a_Height, SWAPCHAIN_IMAGE_NBR);
        graphicsPipeline = CreateGraphicsPipeline(device, renderPass, a_Width, a_Height);
        for (const auto& swapChainImage : SwapChain::GetImages(device, swapChain)) {
            frameBuffers.push_back(CreateFrameBuffer(device, { swapChainImage }));
        }
        presentInfo.swapChains = { swapChain };
    };
    window.OnClose = [&close](const Window&) {
        close = true;
    };
    window.Show();
    while (true) {
        window.PushEvents();
        if (close) break;
        const auto swapChainImageIndex = frameIndex++ % SWAPCHAIN_IMAGE_NBR;
        presentInfo.imageIndices = { swapChainImageIndex };
        const auto swapChainImage = SwapChain::GetImages(device, swapChain).at(swapChainImageIndex);
        if (lastSwapChainImageIndex != swapChainImageIndex || first) {
            first = false;
            lastSwapChainImageIndex = swapChainImageIndex;
            RecordMainCommandBuffer(mainCommandBuffer, frameBuffers.at(swapChainImageIndex), a_Width, a_Height);
        }
        SubmitCommandBuffer(queue, mainCommandBuffer);
        SwapChain::Present(queue, presentInfo);
    }
    return ret;
}
