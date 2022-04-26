#include <Instance.hpp>

#include <GL/Instance.hpp>
#include <GL/PhysicalDevice.hpp>

#include <windows.h>

#include <stdexcept>


namespace OCRA::Instance
{
//for multi instances
static auto s_ID = 0u;
Impl::Impl(const Info& a_Info)
    : info(a_Info)
    , id(s_ID++)
{
    std::string windowClassName = "DummyWindow" + std::to_string(id);
    WNDCLASSEX wndclass{};
    std::memset(&wndclass, 0, sizeof(wndclass));
    wndclass.cbSize = sizeof(wndclass);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = DefWindowProcA;
    wndclass.hInstance = GetModuleHandle(0);
    wndclass.lpszClassName = windowClassName.c_str();
    if (!RegisterClassEx(&wndclass)) throw std::runtime_error("Could not register window class");
    dummyWindow = CreateWindowEx(
        0,
        wndclass.lpszClassName,
        "Dummy Window",
        0,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr, nullptr, wndclass.hInstance, nullptr);
    dummyDevice = GetDC(HWND(dummyWindow));
    physicalDevices.push_back(PhysicalDevice::Create(dummyDevice));
}
Impl::~Impl()
{
    std::string windowClassName = "DummyWindow" + std::to_string(id);
    ReleaseDC(HWND(dummyWindow), HDC(dummyDevice));
    DestroyWindow(HWND(dummyWindow));
    UnregisterClass(windowClassName.c_str(), GetModuleHandle(0));
}
Handle Create(
	const Info& a_Info,
	const AllocationCallback* a_Allocator)
{
    return Handle(new Impl(a_Info));
}
const std::string GetType(const Handle& a_Instance)
{
	return Impl::type;
}
const Info& GetInfo(const Handle& a_Instance)
{
	return a_Instance->info;
}
const std::vector<PhysicalDevice::Handle>& EnumeratePhysicalDevices(const Instance::Handle& a_Instance)
{
	return a_Instance->physicalDevices;
}
}