#include <Common.hpp>

#include <Instance.hpp>
#include <Device.hpp>
#include <Surface.hpp>
#include <SwapChain.hpp>
#include <Command/Pool.hpp>
#include <Command/Buffer.hpp>
#include <Command/Scissor.hpp>
#include <Command/ViewPort.hpp>
#include <Image/Image.hpp>

#include <Windows.h>

using namespace OCRA;

#define SWAPCHAIN_IMAGE_NBR 2

static inline auto RecordClearCommandBuffer(Command::Buffer::Handle a_CommandBuffer, const Image::Handle& a_Image)
{
    Command::Buffer::BeginInfo bufferBeginInfo;
    bufferBeginInfo.flags = Command::Buffer::UsageFlagBits::None;
    Command::Buffer::Reset(a_CommandBuffer);
    Command::Buffer::Begin(a_CommandBuffer, bufferBeginInfo);
    {
        Image::ClearColor clearColor{ 0.35294117f, 0.36862745f, 0.41960784f, 1.f };
        Image::Subresource::Range range{};
        range.levelCount = 1;
        Command::ClearColorImage(a_CommandBuffer, a_Image, Image::Layout::Unknown, clearColor, { range });
    }
    Command::Buffer::End(a_CommandBuffer);
}

#include <Pipeline/Graphics.hpp>

static inline auto CreateGraphicsPipeline(const Device::Handle& a_Device, const uint32_t a_Width, const uint32_t a_Height)
{
    Pipeline::Graphics::Info graphicsPipelineInfo;
    ViewPort viewport;
    viewport.rect.offset = { 0, 0 };
    viewport.rect.extent = { a_Width, a_Height };
    viewport.depthRange  = { 0, 1 };
    iRect2D  scissor;
    scissor.offset = { 0, 0 };
    scissor.extent = { a_Width, a_Height };
    graphicsPipelineInfo.inputAssemblyState.primitiveRestartEnable = false;
    graphicsPipelineInfo.viewPortState.viewPorts = { viewport };
    graphicsPipelineInfo.viewPortState.scissors  = { scissor };
    graphicsPipelineInfo.dynamicState.dynamicStates = { Pipeline::DynamicState::State::Viewport, Pipeline::DynamicState::State::Scissor };
    //Everything else is left by default for now
    return Pipeline::Graphics::Create(a_Device, graphicsPipelineInfo);
}

void RecordMainCommandBuffer(Command::Buffer::Handle& a_MainCommandBuffer, Command::Buffer::Handle& a_ClearCommandBuffer, const uint32_t a_Width, const uint32_t a_Height)
{
    Command::Buffer::BeginInfo beginInfo;
    bufferBeginInfo.flags = Command::Buffer::UsageFlagBits::None;
    ViewPort viewport;
    viewport.rect.offset = { 0, 0 };
    viewport.rect.extent = { a_Width, a_Height };
    viewport.depthRange  = { 0, 1 };
    iRect2D  scissor;
    scissor.offset = { 0, 0 };
    scissor.extent = { a_Width, a_Height };
    Command::Buffer::Reset(a_MainCommandBuffer);
    Command::Buffer::Begin(a_MainCommandBuffer, beginInfo);
    {
        Command::ExecuteCommandBuffer(a_MainCommandBuffer, a_ClearCommandBuffer);
        Command::SetScissor(a_MainCommandBuffer, 0, { scissor });
        Command::SetViewPort(a_MainCommandBuffer, 0, { viewport });
        //draw stuff here
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

    Command::Buffer::Handle mainCommandBuffer  = CreateCommandBuffer(device, commandPool, Command::Pool::AllocateInfo::Level::Primary);
    Command::Buffer::Handle clearCommandBuffer = CreateCommandBuffer(device, commandPool, Command::Pool::AllocateInfo::Level::Secondary);

    SwapChain::Handle       swapChain;
    SwapChain::PresentInfo  presentInfo;
    uint32_t frameIndex = 0;
    bool close = false;

    window.OnResize = [&swapChain, &presentInfo](const Window&, const uint32_t a_Width, const uint32_t a_Height) {
        swapChain = CreateSwapChain(device, surface, swapChain, a_Width, a_Height, SWAPCHAIN_IMAGE_NBR);
        presentInfo.swapChains = { swapChain };
        RecordMainCommandBuffer(mainCommandBuffer, clearCommandBuffer, a_Width, a_Height);
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
        RecordClearCommandBuffer(clearCommandBuffer, swapChainImage);
        SubmitCommandBuffer(queue, mainCommandBuffer);
        SwapChain::Present(queue, presentInfo);
    }
    return ret;
}
