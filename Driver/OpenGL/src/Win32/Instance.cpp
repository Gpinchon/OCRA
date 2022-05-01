#include <GL/Win32/Error.hpp>
#include <GL/Win32/Instance.hpp>
#include <GL/Win32/OpenGL.hpp>
#include <GL/Win32/Window.hpp>
#include <GL/Win32/PhysicalDevice.hpp>
#include <GL/PhysicalDevice.hpp>
#include <GL/Surface.hpp>

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
    dummyWindow = Window::Win32::Create(windowClassName, windowClassName);
}
Impl::~Impl()
{
    std::string windowClassName = "DummyWindow" + std::to_string(id);
    DestroyWindow(HWND(dummyWindow));
    UnregisterClass(windowClassName.c_str(), GetModuleHandle(0));
}

Instance::Handle Create(const Instance::Info& a_Info)
{
    const auto instance = Instance::Handle(new Impl(a_Info));
    Surface::Win32::Info surfaceInfo{};
    surfaceInfo.hinstance = GetModuleHandle(nullptr);
    surfaceInfo.hwnd = std::static_pointer_cast<Instance::Win32::Impl>(instance)->dummyWindow;
    instance->defaultSurface = Surface::Win32::Create(instance, surfaceInfo);
    instance->physicalDevices.push_back(PhysicalDevice::Handle(new PhysicalDevice::Win32::Impl(instance)));
    return instance;
}
}