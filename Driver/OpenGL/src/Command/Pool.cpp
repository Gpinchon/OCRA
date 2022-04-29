#include <GL/Command/Pool.hpp>
#include <GL/WeakHandle.hpp>
#include <GL/Command/Buffer.hpp>

#include <memory>
#include <set>
#include <memory_resource>

namespace OCRA {
struct AllocationCallback;
}

OCRA_DECLARE_HANDLE(OCRA::Command::Buffer);
OCRA_DECLARE_WEAK_HANDLE(OCRA::Command::Pool);

namespace OCRA::Command::Pool
{
struct Impl
{
    Impl(const Device::Handle& a_Device, const Info& a_Info)
        : info(a_Info)
    {}
    std::pmr::synchronized_pool_resource memoryPool;
    std::pmr::polymorphic_allocator<Buffer::Impl>    bufferAllocator(&memoryPool);
    std::pmr::polymorphic_allocator<Buffer::Command> commandAllocator(&memoryPool);
    const Info info;
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
std::vector<Buffer::Handle> AllocateBuffer(const Device::Handle& a_Device, const AllocateInfo& a_Info)
{
    std::vector<Buffer::Handle> commandBuffers;
    commandBuffers.reserve(a_Info.count);
    for (auto i = 0u; i < a_Info.count; ++i) {
        auto buffer = std::allocate_shared<Buffer::Impl>(a_Info.pool->bufferAllocator, a_Info.level);
        commandBuffers.push_back(buffer);
    }
    return commandBuffers;
}
}