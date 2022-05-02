#include <Surface.hpp>
#include <GL/Surface.hpp>
#include <GL/WeakHandle.hpp>
#include <GL/Win32/Surface.hpp>
#include <GL/Win32/Window.hpp>

#include <windows.h>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Instance);

namespace OCRA::Surface::Win32
{
Impl::Impl(const Instance::Handle& a_Instance, const Info& a_Info)
	: Surface::Impl(a_Instance, a_Info.hwnd, "Win32")
	, hdc(GetDC(HWND(nativeWindow)))
{
}
uiExtent2D Impl::GetExtent()
{
	uiExtent2D rect2D;
	RECT windowRect;
    GetWindowRect(
        HWND(nativeWindow),
        &windowRect);
    rect2D.width = windowRect.right - windowRect.left;
    rect2D.height = windowRect.bottom - windowRect.top;
    return rect2D;
}
Impl::~Impl()
{
	ReleaseDC(HWND(nativeWindow), HDC(hdc));
	DeleteDC(HDC(hdc));
}
Handle Create(const Instance::Handle& a_Instance, const Info& a_Info)
{
	return Handle(new Impl(a_Instance, a_Info));
}
}