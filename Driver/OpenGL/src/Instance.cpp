#include <Instance.hpp>

#include <GL/PhysicalDevice.hpp>

#include <string>

namespace OCRA::Instance
{
struct Impl
{
	Impl(const Info& a_Info)
		: info(a_Info)
		, physicalDevices({ PhysicalDevice::Create() }) //OGL has only one device :/
	{}
	const Info info;
	const std::vector<PhysicalDevice::Handle> physicalDevices;
	const std::string type{ "OpenGL" };
};
Handle Create(
	const Info& a_Info,
	const AllocationCallback* a_Allocator)
{
	return Handle(new Impl(a_Info));
}
const std::string& GetType(const Handle& a_Instance)
{
	return a_Instance->type;
}
const Info& GetInfo(const Handle& a_Instance)
{
	return a_Instance->info;
}
const std::vector<PhysicalDevice::Handle>& EnumeratePhysicalDevice(const Instance::Handle& a_Instance)
{
	return a_Instance->physicalDevices;
}
}