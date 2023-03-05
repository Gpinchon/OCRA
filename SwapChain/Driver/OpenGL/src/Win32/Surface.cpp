#include <OCRA/SwapChain/Core.hpp>

#include <GL/Surface.hpp>
#include <GL/Win32/Surface.hpp>
#include <GL/Win32/Window.hpp>

#include <windows.h>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Instance);

namespace OCRA::Instance
{
Surface::Handle CreateSurface(
    const Instance::Handle& a_Instance,
    const CreateSurfaceInfo& a_Info)
{
#ifdef WIN32
    return std::make_shared<Surface::Win32::Impl>(a_Instance, a_Info);
#endif
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

namespace OCRA::Surface::Win32
{
Impl::Impl(const Instance::Handle& a_Instance, const CreateSurfaceInfo& a_Info)
    : Surface::Impl(a_Instance, a_Info.hwnd, "Win32")
    , hdc(GetDC(HWND(nativeWindow)))
{
}
Impl::~Impl()
{
    ReleaseDC(HWND(nativeWindow), HDC(hdc));
    DeleteDC(HDC(hdc));
}
}