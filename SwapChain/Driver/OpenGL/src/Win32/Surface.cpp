#include <OCRA/SwapChain/Core.hpp>

#include <GL/Win32/Surface.hpp>
#include <GL/Win32/Window.hpp>

#include <windows.h>

namespace OCRA::Instance
{
Surface::Handle CreateSurface(
    const Instance::Handle& a_Instance,
    const CreateSurfaceInfo& a_Info)
{
    return std::make_shared<Surface::Impl>(a_Instance, a_Info);
}
}

namespace OCRA::PhysicalDevice
{
std::vector<SurfaceFormat> GetSurfaceFormats(
    const PhysicalDevice::Handle& a_PhysicalDevice,
    const Surface::Handle& a_Surface)
{
    return { {Format::Uint8_Normalized_RGBA, ColorSpace::sRGB} };
}
}

namespace OCRA::Surface
{
Impl::Impl(const Instance::Handle& a_Instance, const CreateSurfaceInfo& a_Info)
    : type("Win32")
    , instance(a_Instance)
    , hwnd(a_Info.hwnd)
    , hdc(GetDC(HWND(hwnd)))
{}

Impl::~Impl()
{
    ReleaseDC(HWND(hwnd), HDC(hdc));
    DeleteDC(HDC(hdc));
}
}