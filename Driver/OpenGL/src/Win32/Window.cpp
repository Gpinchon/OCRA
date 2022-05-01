#include <windows.h>
#include <string>

#include <GL/Win32/Error.hpp>
#include <GL/glew.h>
#include <GL/wglew.h>

namespace OCRA::Window::Win32
{
void* Create(const std::string& a_ClassName, const std::string& a_Name)
{
	const auto hwnd = CreateWindowEx(
        0,
        a_ClassName.c_str(),
        a_Name.c_str(),
        0,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
    WIN32_CHECK_ERROR(hwnd != nullptr);
    MSG msg = { 0 };
    while (PeekMessage(&msg, hwnd, 0, 0, PM_NOYIELD | PM_REMOVE)) {
        DispatchMessage(&msg);
        if (msg.message == WM_CREATE) break;
    }
    return hwnd;
}
void SetDefaultPixelFormat(const void* a_DeviceHandle)
{
	const auto hdc = HDC(a_DeviceHandle);
    const int attribIList[] = {
        WGL_DRAW_TO_WINDOW_ARB, true,
        WGL_SUPPORT_OPENGL_ARB, true,
        WGL_DOUBLE_BUFFER_ARB,  true,
        WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
        WGL_COLORSPACE_EXT,     WGL_COLORSPACE_SRGB_EXT,
        WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB,     32,
        0
    };
    int32_t  pixelFormat = 0;
    uint32_t pixelFormatNbr = 0;
    WIN32_CHECK_ERROR(wglChoosePixelFormatARB(hdc, attribIList, nullptr, 1, &pixelFormat, &pixelFormatNbr));
    WIN32_CHECK_ERROR(pixelFormat != 0);
    WIN32_CHECK_ERROR(pixelFormatNbr != 0);
    WIN32_CHECK_ERROR(SetPixelFormat(hdc, pixelFormat, nullptr));
}
}