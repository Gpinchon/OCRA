#include <OCRA/Core.hpp>

#include <GL/Buffer.hpp>

namespace OCRA::Device
{
Buffer::Handle CreateBuffer(
    const Device::Handle& a_Device,
    const CreateBufferInfo& a_Info,
    const AllocationCallback* a_Allocator)
{
    return std::make_shared<Buffer::Impl>(a_Device, a_Info);
}
}

namespace OCRA::Buffer
{
void BindMemory(const Device::Handle& a_Device, const Handle& a_Buffer, const Memory::Handle& a_Memory, const size_t& a_MemoryOffset)
{
    a_Buffer->memoryBinding.memory = a_Memory;
    a_Buffer->memoryBinding.offset = a_MemoryOffset;
}
}
