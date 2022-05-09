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

struct GraphicsPipelineTestApp
{
    GraphicsPipelineTestApp()
        : window(Window("Test_SwapChain", 1280, 720))
    {
        window.OnResize = [this](const Window&, const uint32_t a_Width, const uint32_t a_Height) {
            OnResize(a_Width, a_Height);
        };
        window.OnClose = [this](const Window&) {
            OnClose();
        };
        instance = CreateInstance("Test_SwapChain");
        surface = CreateSurface(instance, GetModuleHandle(0), (void*)window.nativeHandle);
        physicalDevice = Instance::EnumeratePhysicalDevices(instance).front();
        device = CreateDevice(physicalDevice);
        const auto queueFamily = FindQueueFamily(physicalDevice, PhysicalDevice::QueueFlagsBits::Transfer);
        queue = Device::GetQueue(device, queueFamily, 0); //Get first available queue
        commandPool = CreateCommandPool(device, queueFamily);
        renderPass = CreateRenderPass();
        imageAcquisitionFence = Queue::Fence::Create(device);
        mainCommandBuffer = CreateCommandBuffer(device, commandPool, Command::Pool::AllocateInfo::Level::Primary);
    }
    void Loop()
    {
        while (true) {
            window.PushEvents();
            if (close) break;
            const auto nextImage = SwapChain::AcquireNextImage(device, swapChain, std::chrono::nanoseconds(15000000), nullptr, imageAcquisitionFence);
            Queue::Fence::WaitFor(device, imageAcquisitionFence, std::chrono::nanoseconds(15000000));
            Queue::Fence::Reset(device, { imageAcquisitionFence });
            presentInfo.imageIndices = { nextImage };
            const auto swapChainImage = SwapChain::GetImages(device, swapChain).at(nextImage);
            frameBuffer = CreateFrameBuffer({ swapChainImage });
            RecordMainCommandBuffer();
            SubmitCommandBuffer(queue, mainCommandBuffer);
            SwapChain::Present(queue, presentInfo);
            Queue::WaitIdle(queue);
        }
    }
    RenderPass::Handle CreateRenderPass()
    {
        RenderPass::Info renderPassInfo{};
        //TODO fill this
        return RenderPass::Create(device, renderPassInfo);
    }
    Pipeline::Handle CreateGraphicsPipeline()
    {
        Pipeline::Graphics::Info graphicsPipelineInfo;
        ViewPort viewport;
        viewport.rect.offset = { 0, 0 };
        viewport.rect.extent = extent;
        viewport.depthRange = { 0, 1 };
        Rect2D  scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = extent;
        graphicsPipelineInfo.renderPass = renderPass;
        graphicsPipelineInfo.inputAssemblyState.primitiveRestartEnable = false;
        graphicsPipelineInfo.viewPortState.viewPorts = { viewport };
        graphicsPipelineInfo.viewPortState.scissors = { scissor };
        graphicsPipelineInfo.dynamicState.dynamicStates = { Pipeline::DynamicState::State::Viewport, Pipeline::DynamicState::State::Scissor };
        //Everything else is left by default for now
        return Pipeline::Graphics::Create(device, graphicsPipelineInfo);
    }
    FrameBuffer::Handle CreateFrameBuffer(const std::vector<Image::Handle> a_Attachments)
    {
        FrameBuffer::Info frameBufferInfo{};
        for (const auto& image : a_Attachments) {
            Image::View::Info imageViewInfo;
            imageViewInfo.image = image;
            imageViewInfo.format = Image::Format::Uint8_Normalized_RGBA;
            imageViewInfo.type = Image::View::Type::View2D;
            imageViewInfo.subRange.layerCount = 1;
            const auto imageView = Image::View::Create(device, imageViewInfo);
            frameBufferInfo.attachments.push_back(imageView);
        }
        frameBufferInfo.extent.depth = 1;
        frameBufferInfo.extent.width = extent.width;
        frameBufferInfo.extent.height = extent.height;
        frameBufferInfo.renderPass = renderPass;
        return FrameBuffer::Create(device, frameBufferInfo);
    }
    void RecordMainCommandBuffer()
    {
        Command::Buffer::BeginInfo bufferBeginInfo{};
        bufferBeginInfo.flags = Command::Buffer::UsageFlagBits::None;
        ViewPort viewport;
        viewport.rect.offset = { 0, 0 };
        viewport.rect.extent = extent;
        viewport.depthRange = { 0, 1 };
        Rect2D  scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = extent;
        Command::RenderPassBeginInfo renderPassBeginInfo{};
        Command::SubPassContents     subPassContents{};
        renderPassBeginInfo.renderPass = renderPass;
        renderPassBeginInfo.framebuffer = frameBuffer;
        renderPassBeginInfo.renderArea.offset = { 0, 0 };
        renderPassBeginInfo.renderArea.extent = extent;
        renderPassBeginInfo.clearValues = { {0, 0, 0, 0} };

        Command::Buffer::Reset(mainCommandBuffer);
        Command::Buffer::Begin(mainCommandBuffer, bufferBeginInfo);
        {
            Command::BeginRenderPass(mainCommandBuffer, renderPassBeginInfo, subPassContents);
            {
                Command::SetScissor(mainCommandBuffer, 0, { scissor });
                Command::SetViewPort(mainCommandBuffer, 0, { viewport });
                Command::BindPipeline(mainCommandBuffer, Pipeline::BindingPoint::Graphics, graphicsPipeline);
                //draw stuff here
            }
            Command::EndRenderPass(mainCommandBuffer);
        }
        Command::Buffer::End(mainCommandBuffer);
    }
    void OnResize(const uint32_t a_Width, const uint32_t a_Height)
    {
        swapChain = CreateSwapChain(device, surface, swapChain, a_Width, a_Height, 2);
        graphicsPipeline = CreateGraphicsPipeline();
        presentInfo.swapChains = { swapChain };
        extent = { a_Width, a_Height };
    }
    void OnClose()
    {
        close = true;
    }
    bool                    close{ false };
    Window                  window;
    uExtent2D               extent;
    Instance::Handle        instance;
    Surface::Handle         surface;
    PhysicalDevice::Handle  physicalDevice;
    Device::Handle          device;
    RenderPass::Handle      renderPass;
    FrameBuffer::Handle     frameBuffer;
    Pipeline::Handle        graphicsPipeline;
    SwapChain::Handle       swapChain;
    SwapChain::PresentInfo  presentInfo;
    Command::Pool::Handle   commandPool;
    Command::Buffer::Handle mainCommandBuffer;
    Queue::Handle           queue;
    Queue::Fence::Handle    imageAcquisitionFence;
};

int GraphicsPipeline()
{
    int ret = 0;
    //basic setup as usual
    GraphicsPipelineTestApp testApp;
    testApp.window.Show();
    testApp.Loop();
    return ret;
}
