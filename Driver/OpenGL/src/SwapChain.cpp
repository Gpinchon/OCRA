#include <SwapChain.hpp>

#include <GL/PhysicalDevice.hpp>
#include <GL/Device.hpp>
#include <GL/Image/Format.hpp>
#include <GL/wglew.h>

#include <stdexcept>

namespace OCRA::SwapChain
{
static inline auto GetColorSpaceEGL(const Image::ColorSpace& a_ColorSpace) {
    switch (a_ColorSpace)
    {
        case Image::ColorSpace::Linear:
            return EGL_GL_COLORSPACE_LINEAR;
        case Image::ColorSpace::sRGB:
            return EGL_GL_COLORSPACE_SRGB;
        default : throw std::runtime_error("Unknown ColorSpace");
    }
}
#define FORMATSMAX 32
struct Impl
{
    Impl(const Device::Handle& a_Device, const Info& a_Info)
    {
        const auto hdc = HDC(a_Info.surface->nativeDisplay);
        const auto hglrc = HGLRC(a_Device->physicalDevice.lock()->contextHandle);
        const auto rBits = GetRedSize(a_Info.imageFormat);
        const auto gBits = GetGreenSize(a_Info.imageFormat);
        const auto bBits = GetBlueSize(a_Info.imageFormat);
        const auto aBits = GetAlphaSize(a_Info.imageFormat);
        const auto glDataType = GetGLDataType(a_Info.imageFormat);
        const auto fRGBA = glDataType == GL_HALF_FLOAT || glDataType == GL_FLOAT;
        const auto sRGB = a_Info.imageColorSpace == Image::ColorSpace::sRGB;
        const int attribIList[] = {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
            WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
            WGL_COLORSPACE_EXT,     sRGB  ? WGL_COLORSPACE_SRGB_EXT : WGL_COLORSPACE_LINEAR_EXT,
            WGL_PIXEL_TYPE_ARB,     fRGBA ? WGL_TYPE_RGBA_FLOAT_ARB : WGL_TYPE_RGBA_ARB,
            WGL_COLOR_BITS_ARB,     rBits + gBits + bBits + aBits,
            WGL_RED_BITS_EXT,       rBits,
            WGL_GREEN_BITS_EXT,     gBits,
            WGL_BLUE_BITS_EXT,      bBits,
            WGL_ALPHA_BITS_EXT,     aBits,
            WGL_DEPTH_BITS_EXT,     GetDepthSize(a_Info.imageFormat),
            WGL_STENCIL_BITS_EXT,   GetStencilSize(a_Info.imageFormat),
            0
        };
        const float attribFList[] = {
            0
        };
        int32_t  wglFormats[FORMATSMAX];
        uint32_t wglFormatsNbr = 0;
        wglChoosePixelFormatARB(hdc,
                                attribIList,
                                attribFList,
                                FORMATSMAX,
                                wglFormats,
                                &wglFormatsNbr);
        if (wglFormatsNbr == 0) throw std::runtime_error("Could not find pixel format");
        PIXELFORMATDESCRIPTOR pfd[FORMATSMAX];
        for (auto index = 0u; index < wglFormatsNbr; ++index)
            DescribePixelFormat(hdc, wglFormats[index], sizeof(PIXELFORMATDESCRIPTOR), &pfd[index]);
        SetPixelFormat(hdc, wglFormats[0], &pfd[0]);

        auto instance = a_Device->physicalDevice.lock()->instance.lock();

        a_Device->PushCommand(0, 0, [hdc, hglrc] {
            wglMakeCurrent(hdc, hglrc);
        }, true);
        ReleaseDC(HDC(instance->displayHandle));
        DeleteDC(HDC(instance->displayHandle));
        instance->displayHandle = hdc;
    }
}
Handle Create(const Device::Handle& a_Device, const Info& a_Info)
{
    return Handle(new Impl(a_Device, a_Info));
}
}