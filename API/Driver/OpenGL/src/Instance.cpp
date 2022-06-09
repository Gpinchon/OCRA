#include <Instance.hpp>

#include <GL/Instance.hpp>
#include <GL/Win32/Instance.hpp>

namespace OCRA::Instance
{
//for multi instances
static auto s_ID = 0u;
Impl::Impl(const Info& a_Info)
    : info(a_Info)
    , id(s_ID++)
{
}

Handle Create(
	const Info& a_Info,
	const AllocationCallback* a_Allocator)
{
#ifdef _WIN32
	return Instance::Win32::Create(a_Info);
#endif //_WIN32
}
const std::string GetType(const Handle& a_Instance)
{
	return Impl::type;
}
const Info& GetInfo(const Handle& a_Instance)
{
	return a_Instance->info;
}
const std::vector<PhysicalDevice::Handle>& EnumeratePhysicalDevices(const Instance::Handle& a_Instance)
{
	return a_Instance->physicalDevices;
}
}