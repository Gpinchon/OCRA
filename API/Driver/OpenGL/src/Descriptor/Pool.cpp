#include <OCRA/Descriptor/Pool.hpp>

#include <GL/Descriptor/Set.hpp>
#include <GL/Descriptor/SetLayout.hpp>
#include <GL/Descriptor/Binding.hpp>

#include <memory_resource>

#ifdef _DEBUG
#include <vector>
#endif

OCRA_DECLARE_HANDLE(OCRA::Descriptor::Set);
#ifdef _DEBUG
OCRA_DECLARE_WEAK_HANDLE(OCRA::Descriptor::Set);
#endif

namespace OCRA {
struct AllocationCallback;
}

namespace OCRA::Descriptor::Pool
{
auto GetTypeCount(const Info& a_Info, const Type& a_Type) {
    size_t count = 0;
    for (const auto& size : a_Info.sizes) {
        count += size.type == a_Type ? size.count : 0;
    }
    return count;
}

struct Impl
{
    Impl(const Device::Handle& a_Device, const Info& a_Info)
    {}
#ifdef _DEBUG
    ~Impl() {
        //If this assert fails, this pool was destroyed before the objects it allocated
        for (auto& allocated : allocated) assert(allocated.expired());
    }
#endif
    std::pmr::unsynchronized_pool_resource memory_resource;
#ifdef _DEBUG
    std::vector<Set::WeakHandle>  allocated;
#endif
};

Handle Create(
    const Device::Handle&       a_Device,
    const Info&                 a_Info,
    const AllocationCallback*   a_Allocator)
{
    return Handle(new Impl(a_Device, a_Info));
}

Set::Handle AllocateSet(const Device::Handle& a_Device, const AllocateInfo& a_Info)
{
    auto& memoryResource = a_Info.pool->memory_resource;
    auto set = std::allocate_shared<Set::Impl>(std::pmr::polymorphic_allocator<Set::Impl>(&memoryResource));
    for (auto& binding : a_Info.layout->bindings) {
        for (size_t bindingIndex = 0; bindingIndex < binding.count; ++bindingIndex) {
            auto setBinding = std::allocate_shared<Binding>(std::pmr::polymorphic_allocator<Binding>(&memoryResource), binding.type, binding.binding + bindingIndex);
            set->bindings.at(bindingIndex) = setBinding;
            set->bindingCount++;
        }
    }
#ifdef _DEBUG
    auto& allocated = a_Info.pool->allocated;
    allocated.push_back(set);
    //cleanup while we're at it
    allocated.erase(std::remove_if(allocated.begin(), allocated.end(), [](auto& allocated) { return allocated.expired(); }), allocated.end());
#endif
    return set;
}
}