#include <OCRA/Buffer.hpp>
#include <OCRA/Allocator.hpp>

#include <GL/Buffer.hpp>

namespace OCRA::Buffer
{
Handle Create(
    const Device::Handle&       a_Device,
    const Info&                 a_Info,
    const AllocationCallback*   a_Allocator)
{
    return Handle(new Impl(a_Device, a_Info));
}
void BindMemory(const Device::Handle& a_Device, const Handle& a_Buffer, const Memory::Handle& a_Memory, const size_t& a_MemoryOffset)
{
    a_Buffer->memoryBinding.memory = a_Memory;
    a_Buffer->memoryBinding.offset = a_MemoryOffset;
}
}
