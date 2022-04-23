#include <Common.hpp>

#include <Surface.hpp>
#include <SwapChain.hpp>
#include <Instance.hpp>

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

int SwapChain()
{
	int ret = 0;
	const auto instance = CreateInstance("Test_SwapChain");
    WNDCLASSA wndclass{};
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndclass.lpfnWndProc = DefWindowProcA;
    wndclass.hInstance = GetModuleHandle(0);
    wndclass.lpszClassName = "TestWindow";
    const auto hwnd = CreateWindowEx(
        0,
        wndclass.lpszClassName,
        "Test Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        0,
        0,
        wndclass.hInstance,
        0);
    const auto surface = CreateSurface(instance, wndclass.hInstance, hwnd);
    const auto physicalDevice = Instance::EnumeratePhysicalDevices(instance).front();
    const auto device = CreateDevice(physicalDevice);
    const auto swapChain = CreateSwapChain(device, surface);
    DestroyWindow(hwnd);
    UnregisterClass(wndclass.lpszClassName, wndclass.hInstance);
	return ret;
}