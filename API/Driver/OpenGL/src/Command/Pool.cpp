#include <OCRA/Command/Pool.hpp>

#include <GL/Command/Buffer.hpp>
#include <GL/Common/FixedSizeMemoryPool.hpp>

#include <memory>
#include <set>
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
constexpr auto MaxCommandBufferNbr = 4096;
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
    const Info info;
    FixedSizeMemoryPool<Buffer::Impl, MaxCommandBufferNbr>  memoryPool;
#ifdef _DEBUG
    std::vector<Buffer::WeakHandle>                 allocated;
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
std::vector<Buffer::Handle> AllocateBuffer(const Device::Handle& a_Device, const AllocateInfo& a_Info)
{
    std::vector<Buffer::Handle> commandBuffers;
    commandBuffers.reserve(a_Info.count);
    auto& memoryPool = a_Info.pool->memoryPool;
    for (auto i = 0u; i < a_Info.count; ++i) {
        auto buffer = std::shared_ptr<Buffer::Impl>(new(memoryPool.allocate()) Buffer::Impl(a_Device, Buffer::Level(a_Info.level)), memoryPool.deleter());
        commandBuffers.push_back(buffer);
#ifdef _DEBUG
        auto& allocated = a_Info.pool->allocated;
        allocated.push_back(buffer);
        //cleanup while we're at it
        allocated.erase(std::remove_if(allocated.begin(), allocated.end(), [](auto& allocated) { return allocated.expired(); }), allocated.end());
#endif
    }
    return commandBuffers;
}
}