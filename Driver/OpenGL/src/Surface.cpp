#include <Surface.hpp>

#include <GL/Instance.hpp>
#include <GL/WeakHandle.hpp>

#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

OCRA_DECLARE_WEAK_HANDLE(OCRA::Instance);

namespace OCRA::Surface
{
struct Impl
{
	Impl(const std::string& a_Type) : type(a_Type) {}
	const std::string type;
};
}

namespace OCRA::Surface::Win32
{
#ifdef _WIN32
struct Impl : Surface::Impl
{
	Impl(const Instance::Handle& a_Instance, const Info& a_Info)
		: Surface::Impl("Win32")
		, info(a_Info)
		, instance(a_Instance)
	{
		/*a_Instance->PushCommand([hwnd = HWND(a_Info.hwnd), hinstance = HINSTANCE(a_Info.hinstance)]() {
			auto hdc = GetDC(hwnd);
			wglMakeCurrent(hdc, hglrc);
		});*/
		Instance::MakeCurrent(a_Instance, GetDC(HWND(a_Info.hwnd)));
	}
	const Info info;
	const Instance::WeakHandle instance;
};
Handle CreateWin32(const Instance::Handle& a_Instance, const Info& a_Info)
{
	return Handle(new Impl(a_Instance, a_Info));
}
#endif
}