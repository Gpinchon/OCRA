#include <Instance.hpp>

#include <GL/Instance.hpp>
#include <GL/PhysicalDevice.hpp>
#include <GL/Common/Error.hpp>
#include <GL/glew.h>
#include <GL/wglew.h>

#include <windows.h>
#include <stdexcept>

static bool windowCreated = false;

LRESULT CALLBACK TempWndproc(
    HWND hwnd,        // handle to window
    UINT uMsg,        // message identifier
    WPARAM wParam,    // first message parameter
    LPARAM lParam)    // second message parameter
{
    switch (uMsg)
    {
    case WM_CREATE:
        windowCreated = true;
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

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
    wndclass.lpfnWndProc = TempWndproc;
    wndclass.hInstance = GetModuleHandle(0);
    wndclass.lpszClassName = windowClassName.c_str();
    WIN32_CHECK_ERROR(RegisterClassEx(&wndclass));
    dummyWindow = CreateWindowEx(
        0,
        wndclass.lpszClassName,
        "Dummy Window",
        0,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr, nullptr, wndclass.hInstance, nullptr);
    WIN32_CHECK_ERROR(dummyWindow != nullptr);
    //Wait for the window to be created
    {
        MSG msg = { 0 };
        while (!windowCreated && PeekMessage(&msg, HWND(dummyWindow), 0, 0, PM_NOYIELD | PM_REMOVE))
            DispatchMessage(&msg);
        windowCreated = false;
    }
    dummyDevice = GetDC(HWND(dummyWindow));

    const auto hdc = HDC(dummyDevice);
    PIXELFORMATDESCRIPTOR pfd;
    std::memset(&pfd, 0, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.iLayerType = PFD_MAIN_PLANE;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    const auto pformat = ChoosePixelFormat(hdc, &pfd);
    WIN32_CHECK_ERROR(pformat != 0);
    WIN32_CHECK_ERROR(DescribePixelFormat(hdc, pformat,  sizeof(pfd), &pfd));
    WIN32_CHECK_ERROR(SetPixelFormat(hdc, pformat, &pfd));
    
    glewExperimental = true;
    const auto hglrcTemp = wglCreateContext(hdc);
    WIN32_CHECK_ERROR(hglrcTemp != nullptr);
    WIN32_CHECK_ERROR(wglMakeCurrent(hdc, hglrcTemp));
    WIN32_CHECK_ERROR(glewInit() == GLEW_OK);
    WIN32_CHECK_ERROR(wglewInit() == GLEW_OK);
    wglMakeCurrent(hdc, nullptr);
    wglDeleteContext(hglrcTemp);
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