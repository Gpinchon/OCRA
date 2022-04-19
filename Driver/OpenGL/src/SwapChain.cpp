#include <SwapChain.hpp>

#include <GL/Image/Format.hpp>
#include <GL/eglew.h>

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
struct Impl
{
    Impl(const Device::Handle& a_Device, const Info& a_Info)
    {
        const auto display = a_Info.surface->display;
        const EGLint attribList[] = {
            EGL_RENDERABLE_TYPE,    EGL_TRUE,
            EGL_CONFORMANT,         EGL_OPENGL_BIT,

            EGL_GL_COLORSPACE,      GetColorSpaceEGL(a_Info.imageColorSpace),
            EGL_RED_SIZE,           GetRedSize(a_Info.imageFormat),
            EGL_GREEN_SIZE,         GetGreenSize(a_Info.imageFormat),
            EGL_BLUE_SIZE,          GetBlueSize(a_Info.imageFormat),
            EGL_ALPHA_SIZE,         GetAlphaSize(a_Info.imageFormat),
            EGL_DEPTH_SIZE,         EGL_DONT_CARE,
            EGL_STENCIL_SIZE,       EGL_DONT_CARE,
            EGL_SURFACE_TYPE,       EGL_WINDOW_BIT,
            EGL_NONE
        };
        EGLConfig   config;
        eGLint      configNum;
        eglChooseConfig(display, attribList, &config, 1, &configNum);
        if (eglGetError() != EGL_SUCCESS) throw std::runtime_error("Could not find EGL configuration");
        surface = eglCreateWindowSurface(display, config, a_Info.hwnd, attribList);
        if (eglGetError() != EGL_SUCCESS) throw std::runtime_error("Could not create window surface");
    }
    void *surface;
}
Handle Create(const Device::Handle& a_Device, const Info& a_Info)
{
    return Handle(new Impl(a_Device, a_Info));
}
}