#include <WindowSurface.hpp>

#include <GL/Instance.hpp>
#include <GL/WeakHandle.hpp>

#include <windows.h>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Instance);

namespace OCRA::WindowSurface
{
struct Impl
{
	Impl(const Instance::Handle& a_Instance, const Info& a_Info)
		: info(a_Info)
		, instance(a_Instance)
	{
		Instance::MakeCurrent(a_Instance, GetDC(info.hwnd));
	}
	const Info info;
	const Instance::WeakHandle instance;
};
Handle Create(const Instance::Handle& a_Instance, const Info& a_Info)
{
	return Handle(new Impl(a_Instance, a_Info));
}
}