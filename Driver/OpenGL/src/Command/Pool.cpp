#include <Command/Pool.hpp>

namespace OCRA {
struct AllocationCallback;
}

namespace OCRA::Command::Pool
{
struct Impl
{
	Impl(const Device::Handle& a_Device, const Info& a_Info)
		: info(a_Info)
	{}
	const Info info;
};
Handle Create(
	const Device::Handle&		a_Device,
	const Info&					a_Info,
	const AllocationCallback*	a_Allocator)
{
	return Handle(new Impl(a_Device, a_Info));
}
const Info& GetInfo(const Handle& a_CommandPool)
{
	return a_CommandPool->info;
}
}