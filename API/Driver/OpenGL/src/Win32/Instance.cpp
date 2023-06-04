#include <GL/PhysicalDevice.hpp>
#include <GL/Win32/Error.hpp>
#include <GL/Win32/Instance.hpp>
#include <GL/Win32/OpenGL.hpp>
#include <GL/Win32/PhysicalDevice.hpp>
#include <GL/Win32/Window.hpp>

#include <windows.h>

namespace OCRA::Win32 {
Instance::Handle CreateInstance(const CreateInstanceInfo& a_Info)
{
    const auto instance = std::make_shared<Instance::Win32::Impl>();
    instance->physicalDevices.push_back(std::make_shared<PhysicalDevice::Win32::Impl>(instance));
    return instance;
}
}

namespace OCRA::Instance::Win32 {
Impl::Impl()
    : Instance::Impl()
{
    OpenGL::Win32::Initialize();
    std::string windowClassName = "DummyWindow" + std::to_string(id);
    WNDCLASSEX wndclass {};
    std::memset(&wndclass, 0, sizeof(wndclass));
    wndclass.cbSize        = sizeof(wndclass);
    wndclass.style         = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc   = DefWindowProc;
    wndclass.hInstance     = GetModuleHandle(nullptr);
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
}
