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
bool created = false;
bool recreateSwapChain = true;
uint32_t width = 0;
uint32_t height = 0;

LRESULT CALLBACK Wndproc(
    HWND hwnd,        // handle to window
    UINT uMsg,        // message identifier
    WPARAM wParam,    // first message parameter
    LPARAM lParam)    // second message parameter
{
    switch (uMsg)
    {
    case WM_SIZE :
        width = LOWORD(lParam);
        height = HIWORD(lParam);
        recreateSwapChain = true;
        break;
    case WM_CLOSE:
        close = true;
        break;
    case WM_CREATE:
        created = true;
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int SwapChain()
{
    int ret = 0;
    WNDCLASSEX wndclass{};
    std::memset(&wndclass, 0, sizeof(wndclass));
    wndclass.cbSize = sizeof(wndclass);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = Wndproc;
    wndclass.hInstance = GetModuleHandle(0);
    wndclass.lpszClassName = "TestWindow";
    if (!RegisterClassEx(&wndclass)) {
        std::cerr << "Error in Function = " << __FUNCTION__ << " at line = " << __LINE__ << ", with error code = " << GetLastError() << std::endl;
        throw std::runtime_error("Could not register window class");
    }
    const auto hwnd = CreateWindowEx(
        0,
        wndclass.lpszClassName,
        "Test Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr, nullptr, wndclass.hInstance, nullptr);
    MSG msg = { 0 };
    while (!created && PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
        DispatchMessage(&msg);

    //basic setup as usual
    const auto instance = CreateInstance("Test_SwapChain");
    const auto surface = CreateSurface(instance, wndclass.hInstance, hwnd);
    const auto physicalDevice = Instance::EnumeratePhysicalDevices(instance).front();
    const auto device = CreateDevice(physicalDevice);
    const auto queueFamily = FindQueueFamily(physicalDevice, PhysicalDevice::QueueFlagsBits::Transfer);
    const auto queue = Device::GetQueue(device, queueFamily, 0); //Get first available queue
    const auto commandPool = CreateCommandPool(device, queueFamily);
    //const auto commandBuffer = CreateCommandBuffer(device, commandPool, Command::Pool::AllocateInfo::Level::Primary);

    Command::Buffer::Handle commandBuffer;
    SwapChain::Handle       swapChain;
    SwapChain::PresentInfo  presentInfo;
    uint32_t frameIndex = 0;
    
    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);
    while (!close) {
        while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
            DispatchMessage(&msg);
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
    DestroyWindow(hwnd);
    UnregisterClass(wndclass.lpszClassName, wndclass.hInstance);
	return ret;
}