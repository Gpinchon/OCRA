#include <OCRA/Structs.hpp>

#include <VK/Device.hpp>
#include <VK/Flags.hpp>
#include <VK/Memory.hpp>
#include <VK/PhysicalDevice.hpp>

namespace OCRA::Device
{
Memory::Handle AllocateMemory(
    const Device::Handle& a_Device,
    const AllocateMemoryInfo& a_Info)
{
    auto& device = *a_Device;
    vk::MemoryAllocateInfo info;
    info.allocationSize = a_Info.size;
    info.memoryTypeIndex = a_Info.memoryTypeIndex;
    return std::make_shared<Memory::Impl>(device, info);
}

Memory::Handle AllocateMemory(
    const Handle& a_Device,
    const size_t  a_Size,
    const MemoryPropertyFlags& a_MemoryFlags)
{
    auto& device = *a_Device;
    vk::MemoryAllocateInfo info{ VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
    info.allocationSize = a_Size;
    info.memoryTypeIndex = device.physicalDevice.findMemoryType(ConvertToVk(a_MemoryFlags));
    return std::make_shared<Memory::Impl>(device, info);
}
}

namespace OCRA::Memory
{
void* Map(const MemoryMappedRange& a_MemoryRange)
{
    return a_MemoryRange.memory->mapMemory(a_MemoryRange.offset, a_MemoryRange.length);
}

void Unmap(const Handle& a_Memory)
{
    a_Memory->unmapMemory();
}

void FlushMappedRanges(const std::vector<MemoryMappedRange>& a_Ranges)
{
    const auto& device = a_Ranges.front().memory->device;
    std::vector<vk::MappedMemoryRange> vkRanges;
    vkRanges.reserve(a_Ranges.size());
    for (const auto& range : a_Ranges) {
        vk::MappedMemoryRange vkRange;
        vkRange.memory = **range.memory;
        vkRange.offset = range.offset;
        vkRange.size   = range.length;
        vkRanges.push_back(vkRange);
    }
    device.flushMappedMemoryRanges(vkRanges);
}
}