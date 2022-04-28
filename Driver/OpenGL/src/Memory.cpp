#include <Memory.hpp>
#include <PhysicalDevice.hpp>

#include <GL/Device.hpp>
#include <GL/WeakHandle.hpp>
#include <GL/Memory.hpp>

#include <GL/glew.h>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

#include <semaphore>

namespace OCRA::Memory
{
Impl::Impl(const Device::Handle& a_Device, const Info& a_Info)
    : device(a_Device)
    , info(a_Info)
{
    auto& memoryProperties = PhysicalDevice::GetMemoryProperties(a_Device->physicalDevice.lock());
    auto& memoryTypeFlags = memoryProperties.memoryTypes.at(info.memoryTypeIndex).propertyFlags;
    GLbitfield  allocationFlags{ 0 };
    if ((memoryTypeFlags & PhysicalDevice::MemoryPropertyFlagBits::HostCached) != 0) {
        allocationFlags |= GL_CLIENT_STORAGE_BIT;
    }
    if ((memoryTypeFlags & PhysicalDevice::MemoryPropertyFlagBits::HostVisible) != 0) {
        allocationFlags |= GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;
        mapFlags |= GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT;
    }
    if ((memoryTypeFlags & PhysicalDevice::MemoryPropertyFlagBits::HostCoherent) != 0) {
        allocationFlags |= GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT;
        mapFlags |= GL_MAP_COHERENT_BIT;
    }
    a_Device->PushCommand(0, 0, [this, allocationFlags] {
        glCreateBuffers(1, &handle);
        glNamedBufferStorageEXT(
            handle,
            info.size,
            nullptr,
            allocationFlags);
    }, true);
}
Impl::~Impl()
{
    device.lock()->PushCommand(0, 0, [handle = handle] {
        glDeleteBuffers(1, &handle);
    }, false);
    
}
Handle Allocate(const Device::Handle& a_Device, const Info& a_Info)
{
    return Handle(new Impl(a_Device, a_Info));
}
void* Map(
    const Device::Handle&   a_Device,
    const MappedRange&      a_MemoryRange)
{
    void* ptr{ nullptr };
    a_Device->PushCommand(0, 0, [a_MemoryRange, &ptr] {
        ptr = glMapNamedBufferRangeEXT(
            a_MemoryRange.memory->handle,
            a_MemoryRange.offset,
            a_MemoryRange.length,
            a_MemoryRange.memory->mapFlags);
    }, true);
    return ptr;
}
void Unmap(
    const Device::Handle&  a_Device,
    const Handle&          a_Memory)
{
    a_Device->PushCommand(0, 0, [memory = a_Memory] {
        glUnmapNamedBufferEXT(memory->handle);
    }, false);
}
void FlushMappedRanges(
    const Device::Handle&           a_Device,
    const std::vector<MappedRange>& a_Ranges)
{
    a_Device->PushCommand(0, 0, [ranges = a_Ranges] {
        for (const auto& range : ranges)
            glFlushMappedNamedBufferRangeEXT(
                range.memory->handle,
                range.offset,
                range.length);
    }, false);
}
}