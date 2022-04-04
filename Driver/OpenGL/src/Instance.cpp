#include <Instance.hpp>

#include <GL/Instance.hpp>
#include <GL/PhysicalDevice.hpp>
#include <GL/glew.h>
#include <GL/wglew.h>

#include <stdexcept>
#ifdef _WIN32
#include <windows.h>
#endif

namespace OCRA::Instance
{
Impl::Impl(const Info& a_Info) : info(a_Info)
{
#ifdef _WIN32
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
    const auto hglrcTemp = wglCreateContext(hdc);
    if (hglrcTemp == nullptr) throw std::runtime_error("Failed to create a dummy OpenGL rendering context.");
    if (!wglMakeCurrent(hdc, hglrcTemp)) throw std::runtime_error("Failed to activate dummy OpenGL rendering context.");

    //LET'S GET STARTED !
    if (wglewInit() != GLEW_OK) throw std::runtime_error("Cound not initialize WGLEW");
    if (!WGLEW_ARB_create_context) throw std::runtime_error("Modern context creation not supported !");
    if (!WGLEW_ARB_create_context_robustness) throw std::runtime_error("Robust context creation not supported !");
    int attribs[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_ROBUST_ACCESS_BIT_ARB,
        0
    };
    hglrc = wglCreateContextAttribsARB(hdc, 0, attribs); //commands execution context
    if (hglrc == nullptr) throw std::runtime_error("Failed to create a modern OpenGL rendering context.");
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(hglrcTemp);
#endif
    glThread.PushCommand([this] {
        wglMakeCurrent(hdc, hglrc);
        glewExperimental = TRUE;
        if (glewInit() != GLEW_OK) throw std::runtime_error("Cound not initialize GLEW");
        }, true);
}
Impl::~Impl()
{
    glThread.PushCommand([this] {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(hglrc);
    }, false);
#ifdef _WIN32
    ReleaseDC(hwnd, hdc);
    DestroyWindow(hwnd);
    UnregisterClassA("Dummy_OpenGL_Window", GetModuleHandle(0));
#endif //_WIN32
}

Handle Create(
	const Info& a_Info,
	const AllocationCallback* a_Allocator)
{
    auto instance = Handle(new Impl(a_Info));
    instance->physicalDevices = { PhysicalDevice::Create(instance) };
	return instance;
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
void MakeCurrent(const Handle& a_Instance, const HDC& a_HDC)
{
    a_Instance->glThread.PushCommand([hdc = a_HDC, hglrc = a_Instance->hglrc]{ wglMakeCurrent(hdc, hglrc); }, false);
}
}