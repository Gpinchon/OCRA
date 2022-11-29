#include <Descriptor/Pool.hpp>

#include <GL/Descriptor/Set.hpp>

#include <memory_resource>
#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Descriptor::Set);
#ifdef _DEBUG
OCRA_DECLARE_WEAK_HANDLE(OCRA::Descriptor::Set);
#endif

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
#ifdef _DEBUG
    ~Impl() {
        //If this assert fails, this pool was destroyed before the objects it allocated
        for (auto& allocated : allocated) assert(allocated.expired());
    }
#endif
    const Info                                  info;
    std::vector<std::byte>                      memory{ sizeof(Set::Impl) * info.maxSets };
    std::pmr::monotonic_buffer_resource         memoryPool{ memory.data(), memory.size() };
    std::pmr::polymorphic_allocator<Set::Impl>  allocator{ &memoryPool };
#ifdef _DEBUG
    std::vector<Set::WeakHandle>                allocated;
#endif
};
Handle Create(
    const Device::Handle&       a_Device,
    const Info&                 a_Info,
    const AllocationCallback*   a_Allocator)
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
#ifdef _DEBUG
        auto& allocated = a_Info.pool->allocated;
        allocated.push_back(set);
        //cleanup while we're at it
        allocated.erase(std::remove_if(allocated.begin(), allocated.end(), [](auto& allocated) { return allocated.expired(); }), allocated.end());
#endif
    }
    return sets;
}
}