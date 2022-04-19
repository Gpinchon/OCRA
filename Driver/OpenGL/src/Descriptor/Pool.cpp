#include <Descriptor/Pool.hpp>

#include <GL/Descriptor/Set.hpp>
#include <GL/Common/MemoryPool.hpp>

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
	auto Allocate(const SetLayout::Handle& a_Layout) {
		return new(memoryPool.allocate()) Set::Impl(Set::Level(a_Layout));
	}
	void Deallocate(Set::Impl* a_Ptr) {
		a_Ptr->~Impl();
		memoryPool.free(a_Ptr);
	}
	MemoryPool<Set::Impl, 256> memoryPool;
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
	const auto deallocator = [pool = WeakHandle(a_Info.pool)](Set::Impl* a_Set) { pool.lock()->Deallocate(a_Set); };
	for (const auto& layout : a_Info.layouts) {
		auto set{ a_Info.pool->Allocate(a_Info.level) };
		sets.push_back(Set::Handle(set, deallocator));
	}
	return sets;
}
}