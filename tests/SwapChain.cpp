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

static inline auto CreateSurface(const Instance::Handle& a_Instance, const HINSTANCE& a_HINSTANCE, const HWND& a_HWND) {
    Surface::Win32::Info info{};
    info.hinstance = a_HINSTANCE;
    info.hwnd = a_HWND;
    return Surface::Win32::Create(a_Instance, info);
}

static inline auto CreateSwapChain(const Device::Handle& a_Device, const Surface::Handle& a_Surface)
{
    SwapChain::Info info{};
    info.imageFormat = Image::Format::Int8_Normalized_RGBA;
    info.minImageCount = 2;
    info.surface = a_Surface;
    return SwapChain::Create(a_Device, info);
}

static inline auto ExecuteCommands(const Queue::Handle& a_Queue, const Command::Buffer::Handle& a_CommandBuffer)
{
    
    Queue::SubmitInfo submitInfo;
    submitInfo.commandBuffers.push_back(a_CommandBuffer);
    Queue::Submit(a_Queue, { submitInfo });
}

static inline auto CreateClearCommandBuffer(const Device::Handle& a_Device, Command::Pool::Handle a_CommandPool, const Image::Handle& a_Image)
{
    Command::Pool::AllocateInfo commandBufferAllocateInfo;
    commandBufferAllocateInfo.pool = a_CommandPool;
    commandBufferAllocateInfo.count = 1;
    commandBufferAllocateInfo.level = Command::Pool::AllocateInfo::Level::Primary;
    const auto commandBuffer = Command::Pool::AllocateBuffer(a_Device, commandBufferAllocateInfo).front();
    Command::CommandBufferBeginInfo commandBufferBeginInfo;
    commandBufferBeginInfo.flags = Command::CommandBufferUsageFlagBits::None;
    Command::BeginCommandBuffer(commandBuffer, commandBufferBeginInfo);
    {
        Image::ClearColor clearColor{ 1.f, 0.f, 0.f, 1.f };
        Image::Subresource::Range range{};
        range.levelCount = 1;
        Command::ClearColorImage(commandBuffer, a_Image, Image::Layout::Unknown, clearColor, { range });
    }
    Command::EndCommandBuffer(commandBuffer);
    return commandBuffer;
}

bool close = false;

LRESULT Wndproc(
    HWND hwnd,        // handle to window
    UINT uMsg,        // message identifier
    WPARAM wParam,    // first message parameter
    LPARAM lParam)    // second message parameter
{
    switch (uMsg)
    {
    case WM_CREATE:
        // Initialize the window. 
        return 0;

    case WM_PAINT:
        // Paint the window's client area. 
        return 0;

    case WM_SIZE:
        // Set the size and position of the window. 
        return 0;

    case WM_CLOSE:
        close = true;
        return 0;

        // 
        // Process other messages. 
        // 

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int SwapChain()
{
	int ret = 0;
    WNDCLASSA wndclass{};
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndclass.lpfnWndProc = Wndproc;
    wndclass.hInstance = GetModuleHandle(0);
    wndclass.lpszClassName = "TestWindow";
    if (!RegisterClass(&wndclass)) throw std::runtime_error("Could not register window class");
    const auto hwnd = CreateWindowEx(
        0,
        wndclass.lpszClassName,
        "Test Window",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        0,
        0,
        wndclass.hInstance,
        0);
    const auto instance = CreateInstance("Test_SwapChain");
    const auto physicalDevice = Instance::EnumeratePhysicalDevices(instance).front();
    const auto device = CreateDevice(physicalDevice);
    const auto queueFamily = FindQueueFamily(physicalDevice, PhysicalDevice::QueueFlagsBits::Transfer);
    const auto queue = Device::GetQueue(device, queueFamily, 0); //Get first available queue
    const auto surface = CreateSurface(instance, wndclass.hInstance, hwnd);
    const auto swapChain = CreateSwapChain(device, surface);
    const auto commandPool = CreateCommandPool(device, queueFamily);
    const auto swapChainImage = SwapChain::GetImages(device, swapChain).front();
    const auto commandBuffer = CreateClearCommandBuffer(device, commandPool, swapChainImage);
    ExecuteCommands(queue, commandBuffer);
    SwapChain::PresentInfo presentInfo;
    presentInfo.imageIndices = { 0 };
    presentInfo.swapChains = { swapChain };
    ShowWindow(hwnd, SW_SHOWDEFAULT);
    //UpdateWindow(hwnd);
    MSG msg = { };
    while (!close) {
        // if there's a message for us, then translate it and 
        // dispatch it
        if (GetMessage(&msg, hwnd, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        // otherwise, the window has been closed and isn't
        // receiving any more messages
        else close = true;
        SwapChain::Present(queue, presentInfo);
        _sleep(15);
    }
    DestroyWindow(hwnd);
    UnregisterClass(wndclass.lpszClassName, wndclass.hInstance);
	return ret;
}