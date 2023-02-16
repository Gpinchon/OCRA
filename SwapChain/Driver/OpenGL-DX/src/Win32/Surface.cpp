#include <OCRA/Surface.hpp>
#include <GL-DX/Surface.hpp>
#include <GL-DX/Win32/Surface.hpp>

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