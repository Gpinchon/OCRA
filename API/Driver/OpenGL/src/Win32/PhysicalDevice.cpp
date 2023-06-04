#include <GL/Instance.hpp>
#include <GL/Win32/Error.hpp>
#include <GL/Win32/Instance.hpp>
#include <GL/Win32/OpenGL.hpp>
#include <GL/Win32/PhysicalDevice.hpp>

#include <GL/glew.h>
#include <GL/wglew.h>

namespace OCRA::PhysicalDevice::Win32 {
Impl::Impl(const Instance::Handle& a_Instance)
    : PhysicalDevice::Impl(a_Instance)
    , hdc(GetDC(HWND(std::static_pointer_cast<Instance::Win32::Impl>(a_Instance)->hwnd)))
{
    OpenGL::Win32::SetDefaultPixelFormat(hdc);
    contextHandle = OpenGL::Win32::CreateContext(hdc);
    PushCommand([this] {
        WIN32_CHECK_ERROR(wglMakeCurrent(HDC(hdc), HGLRC(contextHandle)));
    },
        false);
    GetProperties();
}
Impl::~Impl()
{
    PushCommand([] {
        wglMakeCurrent(nullptr, nullptr);
    },
        true);
    WIN32_CHECK_ERROR(wglDeleteContext(HGLRC(contextHandle)));
}
}
