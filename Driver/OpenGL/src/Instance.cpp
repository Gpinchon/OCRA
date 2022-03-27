#include <GL/glew.h>
#include <GL/wglew.h>

#include <Instance.hpp>

#include <GL/PhysicalDevice.hpp>


#include <windows.h>
#include <winuser.h>
#include <stdexcept>
#include <string>

namespace OCRA::Instance
{
struct Impl
{
	Impl(const Info& a_Info)
		: info(a_Info)
	{
#if defined(_WIN32)
        WNDCLASSA wndclass{};
        wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wndclass.lpfnWndProc = DefWindowProcA;
        wndclass.hInstance = GetModuleHandle(0);
        wndclass.lpszClassName = "Dummy_OpenGL_Window";

        //register window class
        if (!RegisterClassA(&wndclass)) throw std::runtime_error("Could not register window class");

        hwnd = CreateWindowExA(
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
        hdc = GetDC(hwnd);

        PIXELFORMATDESCRIPTOR pfd{};
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.cColorBits = 32;
        pfd.cAlphaBits = 8;
        pfd.iLayerType = PFD_MAIN_PLANE;
        pfd.cDepthBits = 24;
        pfd.cStencilBits = 8;
        int pixel_format = ChoosePixelFormat(hdc, &pfd);
        if (!pixel_format) throw std::runtime_error("Failed to find a suitable pixel format.");
        if (!SetPixelFormat(hdc, pixel_format, &pfd)) throw std::runtime_error("Failed to set the pixel format.");
        //gl render context handle
        hglrc = wglCreateContext(hdc);
        if (hglrc == nullptr) throw std::runtime_error("Failed to create a dummy OpenGL rendering context.");
        if (!wglMakeCurrent(hdc, hglrc)) throw std::runtime_error("Failed to activate dummy OpenGL rendering context.");
        if (wglewInit() != GLEW_OK) throw std::runtime_error("Cound not initialize WGLEW");
#endif
        if (glewInit() != GLEW_OK) throw std::runtime_error("Cound not initialize GLEW");
        physicalDevices = { PhysicalDevice::Create() };
    }
    ~Impl()
    {
#if defined(_WIN32)
        wglMakeCurrent(hdc, nullptr);
        wglDeleteContext(hglrc);
        ReleaseDC(hwnd, hdc);
        DestroyWindow(hwnd);
#endif
    }
#if defined(_WIN32)
    HDC         hdc;
    HWND        hwnd;
    HGLRC       hglrc;
#endif
	const Info info;
	std::vector<PhysicalDevice::Handle> physicalDevices;
	const std::string type{ "OpenGL" };
};
Handle Create(
	const Info& a_Info,
	const AllocationCallback* a_Allocator)
{
	return Handle(new Impl(a_Info));
}
const std::string& GetType(const Handle& a_Instance)
{
	return a_Instance->type;
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