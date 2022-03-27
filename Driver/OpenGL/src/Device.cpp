#include <Device.hpp>

namespace OCRA::Device
{
struct Impl
{
	Impl(const PhysicalDevice::Handle& a_PhysicalDevice, const Info& a_Info)
		: info(a_Info)
		, physicalDevice(a_PhysicalDevice)
	{
	}
	const Info info;
	const PhysicalDevice::Handle physicalDevice;
};
Handle Create(const PhysicalDevice::Handle& a_PhysicalDevice, const Info& a_Info, const AllocationCallback* a_Allocator)
{
	return Handle(new Impl(a_PhysicalDevice, a_Info));
}
}