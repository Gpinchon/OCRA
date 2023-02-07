#include <OCRA/Descriptor/Pool.hpp>

#include <GL/Common/MemoryPool.hpp>
#include <GL/Descriptor/Set.hpp>
#include <GL/Descriptor/SetLayout.hpp>
#include <GL/Descriptor/Binding.hpp>

#include <vector>
#include <array>
#include <functional>

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

namespace detail
{

    template <typename T, std::size_t...Is>
    constexpr std::array<T, sizeof...(Is)>
        make_array(std::index_sequence<Is...>, const std::function<T(size_t)>& a_Func)
    {
        return { a_Func(Is)... };
    }
}

template <typename T, std::size_t N>
constexpr std::array<T, N> make_array(const std::function<T(size_t)>& a_Func)
{
    return detail::make_array(std::make_index_sequence<N>(), a_Func);
}

struct Impl
{
    Impl(const Device::Handle& a_Device, const Info& a_Info)
        : sets(a_Info.maxSets)
        , bindings(make_array<MemoryPool<Binding>, size_t(Type::MaxValue)>([a_Info](size_t a_TypeIndex) {
            return GetTypeCount(a_Info, Type(a_TypeIndex));
        }))
    {
    }
#ifdef _DEBUG
    ~Impl() {
        //If this assert fails, this pool was destroyed before the objects it allocated
        for (auto& allocated : allocated) assert(allocated.expired());
    }
#endif
    MemoryPool<Set::Impl>                       sets;
    std::array<MemoryPool<Binding>, size_t(Type::MaxValue)> bindings;
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
    auto& setsPool = a_Info.pool->sets;
    auto set = std::shared_ptr<Set::Impl>(new(setsPool.allocate()) Set::Impl(), setsPool.deleter());
    for (auto& binding : a_Info.layout->bindings) {
        auto& bindingsPool = a_Info.pool->bindings.at(size_t(binding.type));
        for (size_t bindingIndex = 0; bindingIndex < binding.count; ++bindingIndex) {
            auto storage = bindingsPool.allocate();
            auto setBinding = std::shared_ptr<Binding>(new(storage) Binding(binding.type, binding.binding + bindingIndex), bindingsPool.deleter());
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