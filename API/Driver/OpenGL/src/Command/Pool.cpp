#include <OCRA/Command/Pool.hpp>

#include <GL/Command/Buffer.hpp>
#include <GL/Common/FixedSizeMemoryPool.hpp>

#include <memory>
#include <memory_resource>

namespace OCRA {
struct AllocationCallback;
}

OCRA_DECLARE_HANDLE(OCRA::Command::Buffer);
#ifdef _DEBUG
OCRA_DECLARE_WEAK_HANDLE(OCRA::Command::Buffer);
#endif

namespace OCRA::Command::Pool
{
struct Impl
{
    Impl(const Device::Handle& a_Device, const Info& a_Info)
    {}
#ifdef _DEBUG
    ~Impl() {
        //If this assert fails, this pool was destroyed before the objects it allocated
        for (auto& allocated : allocated) assert(allocated.expired());
    }
    std::vector<Buffer::WeakHandle>         allocated;
#endif
    std::pmr::unsynchronized_pool_resource  memoryResource;
};

Handle Create(
    const Device::Handle&       a_Device,
    const Info&                 a_Info,
    const AllocationCallback*   a_Allocator)
{
    return Handle(new Impl(a_Device, a_Info));
}

std::vector<Buffer::Handle> AllocateBuffer(const Device::Handle& a_Device, const AllocateInfo& a_Info)
{
    std::vector<Buffer::Handle> commandBuffers;
    commandBuffers.reserve(a_Info.count);
    auto& memoryResource = a_Info.pool->memoryResource;
    auto allocator = std::pmr::polymorphic_allocator<Buffer::Impl>(&memoryResource);
#ifdef _DEBUG
    auto& allocated = a_Info.pool->allocated;
#endif
    for (auto i = 0u; i < a_Info.count; ++i) {
        auto buffer = std::allocate_shared<Buffer::Impl>(allocator, a_Device, Buffer::Level(a_Info.level), &memoryResource);
        commandBuffers.push_back(buffer);
#ifdef _DEBUG
        allocated.push_back(buffer);
        //cleanup while we're at it
        allocated.erase(std::remove_if(allocated.begin(), allocated.end(), [](auto& allocated) { return allocated.expired(); }), allocated.end());
#endif
    }
    return commandBuffers;
}
}