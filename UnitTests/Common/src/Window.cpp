#include <Window.hpp>

#include <OCRA/Device.hpp>
#include <OCRA/Surface.hpp>
#include <OCRA/SwapChain.hpp>

#include <iostream>
#include <windows.h>

LRESULT CALLBACK TestWndproc(
    HWND hwnd,        // handle to window
    UINT uMsg,        // message identifier
    WPARAM wParam,    // first message parameter
    LPARAM lParam)    // second message parameter
{
    const auto window = (OCRA::Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    if (window == nullptr) return DefWindowProc(hwnd, uMsg, wParam, lParam);
    switch (uMsg)
    {
    case WM_PAINT:
        if (window->OnPaint) window->OnPaint(*window);
        break;
    case WM_SIZE:
        switch (wParam) {
        case SIZE_MAXIMIZED:
            window->ResizeCallback(LOWORD(lParam), HIWORD(lParam));
            if (window->OnMaximize) window->OnMaximize(*window, LOWORD(lParam), HIWORD(lParam));
            break;
        case SIZE_MINIMIZED:
            if (window->OnMinimize) window->OnMinimize(*window, LOWORD(lParam), HIWORD(lParam));
            break;
        case SIZE_RESTORED:
            window->ResizeCallback(LOWORD(lParam), HIWORD(lParam));
            if (window->OnRestore) window->OnRestore(*window, LOWORD(lParam), HIWORD(lParam));
            break;
        default:
            window->ResizeCallback(LOWORD(lParam), HIWORD(lParam));
            if (window->OnResize) window->OnResize(*window, LOWORD(lParam), HIWORD(lParam));
        }
        break;
    case WM_CLOSE:
        window->ClosingCallback();
        if (window->OnClose) window->OnClose(*window);
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

namespace OCRA {
struct WindowClass : WNDCLASSEX
{
    WindowClass()
    {
        std::memset(this, 0, sizeof(WindowClass));
        cbSize = sizeof(WindowClass);
        style = CS_HREDRAW | CS_VREDRAW;
        lpfnWndProc = TestWndproc;
        hInstance = GetModuleHandle(0);
        lpszClassName = "TestWindow";
        if (!RegisterClassEx(this)) {
            std::cerr << "Error in Function = " << __FUNCTION__ << " at line = " << __LINE__ << ", with error code = " << GetLastError() << std::endl;
            throw std::runtime_error("Could not register window class");
        }
    }
    ~WindowClass()
    {
        UnregisterClass(lpszClassName, hInstance);
    }
};

static inline auto& GetWindowClass()
{
    static WindowClass wndclass;
    return wndclass;
}

Window::Window(const Instance::Handle& a_Instance, const PhysicalDevice::Handle& a_PhysicalDevice, const Device::Handle& a_Device, const std::string& name, const uint32_t a_Width, const uint32_t a_Height)
    : device(a_Device)
{
    const auto& wndclass = GetWindowClass();
    const auto hwnd = CreateWindowEx(
        0,
        wndclass.lpszClassName,
        name.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, a_Width, a_Height,
        nullptr, nullptr, wndclass.hInstance, nullptr);
    MSG msg{ 0 };
    while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) {
        DispatchMessage(&msg);
        if (msg.message == WM_CREATE) break;
    }
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);
    nativeHandle = hwnd;
    Surface::Win32::Info info{};
    info.hinstance = GetModuleHandle(0);
    info.hwnd = hwnd;
    surface = Surface::Win32::Create(a_Instance, info);
}

Window::~Window()
{
    DestroyWindow(HWND(nativeHandle));
}

void Window::PushEvents()
{
    MSG msg{ 0 };
    while (PeekMessage(&msg, HWND(nativeHandle), 0, 0, PM_REMOVE))
        DispatchMessage(&msg);
}

void Window::Show()
{
    ShowWindow(HWND(nativeHandle), SW_SHOWDEFAULT);
    UpdateWindow(HWND(nativeHandle));
}

void Window::Update()
{
    UpdateWindow(HWND(nativeHandle));
}

void Window::Present(const Queue::Handle& a_Queue, const Queue::Semaphore::Handle& a_WaitSemaphore)
{
    if (a_WaitSemaphore != nullptr) presentInfo.waitSemaphores = { a_WaitSemaphore };
    SwapChain::Present(a_Queue, presentInfo);
}

Image::Handle Window::AcquireNextImage(
    const std::chrono::nanoseconds& a_Timeout,
    const Queue::Semaphore::Handle& a_Semaphore,
    const Queue::Fence::Handle& a_Fence)
{
    return SwapChain::AcquireNextImage(device, swapChain, a_Timeout, a_Semaphore, a_Fence);
}

void Window::SetSwapChainImageNbr(uint32_t a_ImageNbr)
{
    swapChainImageNbr = a_ImageNbr;
    ResizeCallback(GetExtent().width, GetExtent().height);
}

void Window::SetVSync(bool a_VSync)
{
    vSync = a_VSync;
    ResizeCallback(GetExtent().width, GetExtent().height);
}

void Window::ResizeCallback(const uint32_t a_Width, const uint32_t a_Height)
{
    if (IsClosing()) return;
    SwapChain::Info info{};
    info.oldSwapchain = swapChain;
    info.presentMode = vSync ? SwapChain::PresentMode::Fifo : SwapChain::PresentMode::Immediate;
    info.imageColorSpace = Image::ColorSpace::sRGB;
    info.imageFormat = Image::Format::Uint8_Normalized_RGBA;
    info.imageCount = GetSwapChainImageNbr();
    info.surface = surface;
    info.imageExtent.width = a_Width;
    info.imageExtent.height = a_Height;
    if (a_Width > 0 && a_Height > 0)
        swapChain = SwapChain::Create(device, info);
    extent = { a_Width, a_Height };
    presentInfo.swapChains = { swapChain };
}
}