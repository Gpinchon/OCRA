#include <GL/Instance.hpp>
#include <GL/Win32/PhysicalDevice.hpp>
#include <GL/Win32/OpenGL.hpp>
#include <GL/Win32/Error.hpp>
#include <GL/Win32/Surface.hpp>

#include <GL/glew.h>
#include <GL/wglew.h>

namespace OCRA::PhysicalDevice::Win32
{
Impl::Impl(const Instance::Handle& a_Instance)
    : PhysicalDevice::Impl(Type::Win32, a_Instance, OpenGL::Win32::CreateContext(std::static_pointer_cast<Surface::Win32::Impl>(a_Instance->defaultSurface)->hdc))
{
    ResetSurface();
    GetProperties();
}
Impl::~Impl()
{
    PushCommand([] {
        wglMakeCurrent(nullptr, nullptr);
    }, true);
    WIN32_CHECK_ERROR(wglDeleteContext(HGLRC(contextHandle)));
}
void Impl::SetSurface(const Surface::Handle& a_Surface)
{
    const auto hdc = HDC(std::static_pointer_cast<Surface::Win32::Impl>(currentSurface.lock())->hdc);
    PushCommand([this, hdc, newSurface = a_Surface] {
        if (newSurface != currentSurface) {
            currentSurface.onDestroy = {};
            currentSurface = newSurface;
            currentSurface.onDestroy = [this](Surface::Impl*){ ResetSurface(); };
            WIN32_CHECK_ERROR(wglMakeCurrent(hdc, HGLRC(contextHandle)));
        }
    }, false);
}
void Impl::SwapBuffers(const uint8_t a_SwapInterval)
{
    const auto hdc = HDC(std::static_pointer_cast<Surface::Win32::Impl>(currentSurface.lock())->hdc);
    PushCommand([this, hdc, newSwapInterval = a_SwapInterval] {
        if (newSwapInterval != swapInterval) {
            swapInterval = newSwapInterval;
            wglSwapIntervalEXT(swapInterval);
        }
        wglSwapLayerBuffers(hdc, WGL_SWAP_MAIN_PLANE);
    }, true);
}
}