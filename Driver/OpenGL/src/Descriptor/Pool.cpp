#include <Descriptor/Pool.hpp>

#include <GL/Descriptor/Set.hpp>

#include <memory_resource>
#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Descriptor::Set);
OCRA_DECLARE_WEAK_HANDLE(OCRA::Descriptor::Pool);

namespace OCRA {
struct AllocationCallback;
}

namespace OCRA::Descriptor::Pool
{
struct Impl
{
	Impl(const Device::Handle& a_Device, const Info& a_Info)
		: info(a_Info)
	{}
	std::pmr::unsynchronized_pool_resource		memoryPool;
	std::pmr::polymorphic_allocator<Set::Impl>	allocator{ &memoryPool };
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
std::vector<Set::Handle> AllocateSet(const Device::Handle& a_Device, const AllocateInfo& a_Info)
{
	std::vector<Set::Handle> sets;
	sets.reserve(a_Info.layouts.size());
	for (const auto& layout : a_Info.layouts) {
		auto set = std::allocate_shared<Set::Impl>(a_Info.pool->allocator, layout);
		sets.push_back(set);
	}
	return sets;
}
}