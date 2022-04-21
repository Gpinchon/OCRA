#include <Instance.hpp>

#include <GL/Instance.hpp>
#include <GL/PhysicalDevice.hpp>

#include <windows.h>

#include <stdexcept>


namespace OCRA::Instance
{
Impl::Impl(const Info& a_Info) : info(a_Info)
{
    //if (eglewInit() != GLEW_OK) throw std::runtime_error("Cound not initialize EGLEW");
    //constexpr EGLint maxDevices = 16;
    //EGLDeviceEXT    devices[maxDevices];
    //EGLint          devicesNum;
    //eglQueryDevicesEXT(maxDevices, devices, &devicesNum);
    //if (eglGetError() != EGL_SUCCESS) throw std::runtime_error("Could not query devices");
    //physicalDevices.push_back(PhysicalDevice::Create(eglGetDisplay(EGL_DEFAULT_DISPLAY)));
    /*for (auto deviceIndex = 0; deviceIndex < devicesNum; ++deviceIndex) {
        physicalDevices.push_back(PhysicalDevice::Create(devices[deviceIndex]));
    }*/
#ifdef _WIN32
    WNDCLASSA wndclass{};
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndclass.lpfnWndProc = DefWindowProcA;
    wndclass.hInstance = GetModuleHandle(0);
    wndclass.lpszClassName = "Dummy_OpenGL_Window";
    if (!RegisterClassA(&wndclass)) throw std::runtime_error("Could not register window class");

    auto hwnd = CreateWindowExA(
        0,
        wndclass.lpszClassName,
        "Dummy OpenGL Window",
        0,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        0,
        0,
        wndclass.hInstance,
        0);

    //device context handle
    displayHandle = GetDC(HWND(hwnd));
#endif //_WIN32
    physicalDevices.push_back(PhysicalDevice::Create(displayHandle));
    DestroyWindow(HWND(hwnd));
    UnregisterClassA("Dummy_OpenGL_Window", GetModuleHandle(0));
}
Impl::~Impl()
{
#if _WIN32
    DeleteDC(HDC(displayHandle));
    UnregisterClassA("Dummy_OpenGL_Window", GetModuleHandle(0));
#endif //_WIN32
}
Handle Create(
	const Info& a_Info,
	const AllocationCallback* a_Allocator)
{
	return Handle(new Impl(a_Info));
}
const std::string& GetType(const Handle& a_Instance)
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