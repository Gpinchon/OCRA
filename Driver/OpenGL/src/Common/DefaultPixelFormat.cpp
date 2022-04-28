#include <GL/Common/Error.hpp>
#include <GL/glew.h>
#include <GL/wglew.h>
#include <windows.h>

namespace OCRA::Win32
{
void SetDefaultPixelFormat(const void* a_DeviceHandle)
{
    const auto hdc = HDC(a_DeviceHandle);
    PIXELFORMATDESCRIPTOR pfd;
    std::memset(&pfd, 0, sizeof(pfd));
    /*
    const int attribIList[] = {
        WGL_DRAW_TO_WINDOW_ARB, true,
        WGL_SUPPORT_OPENGL_ARB, true,
        WGL_DOUBLE_BUFFER_ARB,  true,
        WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
        WGL_COLORSPACE_EXT,     WGL_COLORSPACE_LINEAR_EXT,
        WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB,     32,
        WGL_DEPTH_BITS_EXT,     24,
        WGL_STENCIL_BITS_EXT,   8,
        0
    };
    */
    const int attribIList[] = {
        WGL_DRAW_TO_WINDOW_ARB, true,
        WGL_SUPPORT_OPENGL_ARB, true,
        WGL_DOUBLE_BUFFER_ARB,  true,
        WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
        WGL_STENCIL_BITS_EXT,   8,
        0
    };
    int32_t  pixelFormat = 0;
    uint32_t pixelFormatNbr = 0;
    WIN32_CHECK_ERROR(wglChoosePixelFormatARB(hdc, attribIList, nullptr, 1, &pixelFormat, &pixelFormatNbr));
    WIN32_CHECK_ERROR(pixelFormat != 0);
    WIN32_CHECK_ERROR(pixelFormatNbr != 0);
    WIN32_CHECK_ERROR(SetPixelFormat(hdc, pixelFormat, &pfd));
}
}