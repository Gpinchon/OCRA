#include <GL/Win32/Error.hpp>
#include <GL/Win32/Instance.hpp>
#include <GL/Win32/OpenGL.hpp>
#include <GL/Win32/Window.hpp>
#include <GL/Win32/PhysicalDevice.hpp>
#include <GL/PhysicalDevice.hpp>

#include <windows.h>

namespace OCRA::Instance::Win32
{
Impl::Impl(const Instance::Info& a_Info)
    : Instance::Impl(a_Info)
{
    OpenGL::Win32::Initialize();
    std::string windowClassName = "DummyWindow" + std::to_string(id);
    WNDCLASSEX wndclass{};
    std::memset(&wndclass, 0, sizeof(wndclass));
    wndclass.cbSize = sizeof(wndclass);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = DefWindowProc;
    wndclass.hInstance = GetModuleHandle(nullptr);
    wndclass.lpszClassName = windowClassName.c_str();
    WIN32_CHECK_ERROR(RegisterClassEx(&wndclass));
    hwnd = Window::Win32::Create(windowClassName, windowClassName);
}
Impl::~Impl()
{
    std::string windowClassName = "DummyWindow" + std::to_string(id);
    DestroyWindow(HWND(hwnd));
    UnregisterClass(windowClassName.c_str(), GetModuleHandle(0));
}

Instance::Handle Create(const Instance::Info& a_Info)
{
    const auto instance = Instance::Handle(new Impl(a_Info));
    instance->physicalDevices.push_back(PhysicalDevice::Handle(new PhysicalDevice::Win32::Impl(instance)));
    return instance;
}
}