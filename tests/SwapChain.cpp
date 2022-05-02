#include <Common.hpp>

#include <Instance.hpp>
#include <Device.hpp>
#include <Surface.hpp>
#include <SwapChain.hpp>
#include <Command/Pool.hpp>
#include <Command/Buffer.hpp>
#include <Image/Image.hpp>

#include <Windows.h>

using namespace OCRA;

#define SWAPCHAIN_IMAGE_NBR 2

static inline auto RecordClearCommandBuffer(Command::Buffer::Handle a_CommandBuffer, const Image::Handle& a_Image)
{
    Command::Buffer::BeginInfo bufferBeginInfo;
    bufferBeginInfo.flags = Command::Buffer::UsageFlagBits::None;
    Command::Buffer::Begin(a_CommandBuffer, bufferBeginInfo);
    {
        Image::ClearColor clearColor{ 1.f, 0.f, 0.f, 1.f };
        Image::Subresource::Range range{};
        range.levelCount = 1;
        Command::ClearColorImage(a_CommandBuffer, a_Image, Image::Layout::Unknown, clearColor, { range });
    }
    Command::Buffer::End(a_CommandBuffer);
}

bool close = false;
bool recreateSwapChain = true;
uint32_t width = 0;
uint32_t height = 0;

int SwapChain()
{
    int ret = 0;
    auto window = Window("Test_SwapChain", 1280, 720);
    window.OnResize = [](const Window&, const uint32_t a_Width, const uint32_t a_Height){
        recreateSwapChain = true;
        width = a_Width;
        height = a_Height;
    };
    window.OnClose = [](const Window&) {
        close = true;
    };
    //basic setup as usual
    const auto instance = CreateInstance("Test_SwapChain");
    const auto surface = CreateSurface(instance, GetModuleHandle(0), (void*)window.nativeHandle);
    const auto physicalDevice = Instance::EnumeratePhysicalDevices(instance).front();
    const auto device = CreateDevice(physicalDevice);
    const auto queueFamily = FindQueueFamily(physicalDevice, PhysicalDevice::QueueFlagsBits::Transfer);
    const auto queue = Device::GetQueue(device, queueFamily, 0); //Get first available queue
    const auto commandPool = CreateCommandPool(device, queueFamily);

    Command::Buffer::Handle commandBuffer;
    SwapChain::Handle       swapChain;
    SwapChain::PresentInfo  presentInfo;
    uint32_t frameIndex = 0;
    
    window.Show();
    while (!close) {
        window.PushEvents();
        const auto swapChainImageIndex = frameIndex++ % SWAPCHAIN_IMAGE_NBR;
        presentInfo.imageIndices = { swapChainImageIndex };
        if (recreateSwapChain)
        {
            recreateSwapChain = false;
            swapChain = CreateSwapChain(device, surface, swapChain, width, height, SWAPCHAIN_IMAGE_NBR);
            presentInfo.swapChains = { swapChain };
        }
        const auto swapChainImage = SwapChain::GetImages(device, swapChain).at(swapChainImageIndex);
        if (commandBuffer == nullptr)
            commandBuffer = CreateCommandBuffer(device, commandPool, Command::Pool::AllocateInfo::Level::Primary);
        else Command::Buffer::Reset(commandBuffer);
        RecordClearCommandBuffer(commandBuffer, swapChainImage);
        SubmitCommandBuffer(queue, commandBuffer);
        SwapChain::Present(queue, presentInfo);
    }
	return ret;
}
