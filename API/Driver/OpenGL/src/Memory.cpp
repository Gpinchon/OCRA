#include <OCRA/OCRA.hpp>

#include <GL/Device.hpp>
#include <GL/PhysicalDevice.hpp>
#include <GL/Memory.hpp>

#include <GL/glew.h>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Device
{
Memory::Handle AllocateMemory(const Device::Handle& a_Device, const AllocateMemoryInfo& a_Info)
{
    return std::make_shared<Memory::Impl>(a_Device, a_Info);
}
}

namespace OCRA::Memory
{
Impl::Impl(const Device::Handle& a_Device, const AllocateMemoryInfo& a_Info)
    : device(a_Device)
    , size(a_Info.size)
{
    const auto physicalDevice = a_Device->physicalDevice.lock();
    const auto& memoryProperties = physicalDevice->memoryProperties;
    const auto& memoryTypeFlags = memoryProperties.memoryTypes.at(a_Info.memoryTypeIndex).propertyFlags;
    GLbitfield  allocationFlags{ 0 };
    if ((memoryTypeFlags & MemoryPropertyFlagBits::HostCached) != 0) {
        allocationFlags |= GL_CLIENT_STORAGE_BIT;
    }
    if ((memoryTypeFlags & MemoryPropertyFlagBits::HostVisible) != 0) {
        allocationFlags |= GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;
        mapFlags |= GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT;
    }
    if ((memoryTypeFlags & MemoryPropertyFlagBits::HostCoherent) != 0) {
        allocationFlags |= GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT;
        mapFlags |= GL_MAP_COHERENT_BIT;
    }
    a_Device->PushCommand([this, allocationFlags] {
        glCreateBuffers(1, &handle);
        glNamedBufferStorageEXT(
            handle,
            size,
            nullptr,
            allocationFlags);
    }, true);
}
Impl::~Impl()
{
    device.lock()->PushCommand([handle = handle] {
        glDeleteBuffers(1, &handle);
    }, false);
    
}

void* Map(const MemoryMappedRange& a_MemoryRange)
{
    void* ptr{ nullptr };
    auto device = a_MemoryRange.memory->device.lock();
    device->PushCommand([a_MemoryRange, &ptr] {
        ptr = glMapNamedBufferRangeEXT(
            a_MemoryRange.memory->handle,
            a_MemoryRange.offset,
            a_MemoryRange.length,
            a_MemoryRange.memory->mapFlags);
    }, true);
    return ptr;
}
void Unmap(const Handle&          a_Memory)
{
    auto device = a_Memory->device.lock();
    device->PushCommand([memory = a_Memory] {
        glUnmapNamedBufferEXT(memory->handle);
    }, false);
}
void FlushMappedRanges(const std::vector<MemoryMappedRange>& a_Ranges)
{
    if (!a_Ranges.empty()) {
        Device::Handle device = a_Ranges.front().memory->device.lock();
        device->PushCommand([ranges = a_Ranges] {
            for (const auto& range : ranges)
                glFlushMappedNamedBufferRangeEXT(
                    range.memory->handle,
                    range.offset,
                    range.length);
            }, false);
    }
}
}