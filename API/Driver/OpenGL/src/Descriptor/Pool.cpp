#include <OCRA/Descriptor/Pool.hpp>

#include <GL/Common/MemoryPool.hpp>
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
    MemoryPool<Set::Impl>                       memoryPool{ info.maxSets };
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
Set::Handle AllocateSet(const Device::Handle& a_Device, const AllocateInfo& a_Info)
{
    auto& memoryPool = a_Info.pool->memoryPool;
    auto set = std::shared_ptr<Set::Impl>(new(memoryPool.allocate()) Set::Impl(a_Info.layout), memoryPool.deleter());
#ifdef _DEBUG
    auto& allocated = a_Info.pool->allocated;
    allocated.push_back(set);
    //cleanup while we're at it
    allocated.erase(std::remove_if(allocated.begin(), allocated.end(), [](auto& allocated) { return allocated.expired(); }), allocated.end());
#endif
    return set;
}
}