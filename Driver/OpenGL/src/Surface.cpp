#include <Surface.hpp>

#include <GL/Instance.hpp>
#include <GL/WeakHandle.hpp>
#include <GL/Surface.hpp>

#include <string>
#include <stdexcept>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Instance);

#ifdef _WIN32
#include <windows.h>

namespace OCRA::Surface::Win32
{
Impl::Impl(const Instance::Handle& a_Instance, const Info& a_Info)
	: Surface::Impl(a_Instance, a_Info.hwnd, "Win32")
{
}
Handle Create(const Instance::Handle& a_Instance, const Info& a_Info)
{
	return Handle(new Impl(a_Instance, a_Info));
}
}
#endif
