#include <OCRA/Core.hpp>

#include <GL/DescriptorBinding.hpp>
#include <GL/DescriptorSet.hpp>
#include <GL/DescriptorSetLayout.hpp>

#include <memory_resource>

OCRA_DECLARE_HANDLE(OCRA::Descriptor::Set);
#ifdef _DEBUG
OCRA_DECLARE_WEAK_HANDLE(OCRA::Descriptor::Set);
#endif

namespace OCRA::Device {
Descriptor::Pool::Handle CreateDescriptorPool(
    const Device::Handle& a_Device,
    const CreateDescriptorPoolInfo& a_Info,
    const AllocationCallback* a_Allocator)
{
    return std::make_shared<Descriptor::Pool::Impl>(a_Device, a_Info);
}
}

namespace OCRA::Descriptor::Pool {
auto GetTypeCount(const CreateDescriptorPoolInfo& a_Info, const DescriptorType& a_Type)
{
    size_t count = 0;
    for (const auto& size : a_Info.sizes) {
        count += size.type == a_Type ? size.count : 0;
    }
    return count;
}

struct Impl {
    Impl(const Device::Handle& a_Device, const CreateDescriptorPoolInfo& a_Info)
    {
    }
    std::pmr::unsynchronized_pool_resource memory_resource;

#ifdef _DEBUG
    ~Impl()
    {
        // If this assert fails, this pool was destroyed before the objects it allocated
        for (auto& allocated : allocated)
            assert(allocated.expired());
    }
    std::pmr::vector<Set::WeakHandle> allocated { &memory_resource };
#endif
};

Set::Handle AllocateSet(
    const Handle& a_Pool,
    const AllocateDescriptorSetInfo& a_Info)
{
    size_t bindingCount  = 0;
    const auto& bindings = SetLayout::CreateDirectIndexedLayout(a_Info.bindings);
    for (auto& binding : bindings) {
        bindingCount += binding.count;
    }
    auto& memoryResource = a_Pool->memory_resource;
    auto set             = std::allocate_shared<Set::Impl>(std::pmr::polymorphic_allocator<Set::Impl>(&memoryResource), &memoryResource);
    set->bindings.reserve(bindingCount);
    for (auto& binding : bindings) {
        for (size_t bindingIndex = 0; bindingIndex < binding.count; ++bindingIndex) {
            set->bindings.push_back(Binding(binding.type, binding.binding + bindingIndex));
        }
    }
#ifdef _DEBUG
    auto& allocated = a_Pool->allocated;
    allocated.push_back(set);
    // cleanup while we're at it
    allocated.erase(std::remove_if(allocated.begin(), allocated.end(), [](auto& allocated) { return allocated.expired(); }), allocated.end());
#endif
    return set;
}
}
