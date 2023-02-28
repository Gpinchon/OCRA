#include <VK/Device.hpp>
#include <VK/Memory.hpp>

#include <OCRA/Structs.hpp>

namespace OCRA::Device
{
Memory::Handle AllocateMemory(const Device::Handle& a_Device, const AllocateMemoryInfo& a_Info)
{
    VkDeviceMemory memory = nullptr;
    VkMemoryAllocateInfo info{ VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
    info.allocationSize = a_Info.size;
    info.memoryTypeIndex = a_Info.memoryTypeIndex;
    vkAllocateMemory(*a_Device, &info, nullptr, &memory);
    return std::make_shared<Memory::Impl>(*a_Device, memory);
}
}

namespace OCRA::Memory
{
void* Map(const MemoryMappedRange& a_MemoryRange)
{
    void* ptr = nullptr;
    const auto& memory = *a_MemoryRange.memory;
    vkMapMemory(memory.device, memory,
        a_MemoryRange.offset, a_MemoryRange.length,
        0, &ptr);
    return ptr;
}

void Unmap(const Handle& a_Memory)
{
    const auto& memory = *a_Memory;
    vkUnmapMemory(memory.device, memory);
}

void FlushMappedRanges(const std::vector<MemoryMappedRange>& a_Ranges)
{
    const auto& device = a_Ranges.front().memory->device;
    std::vector<VkMappedMemoryRange> vkRanges;
    vkRanges.reserve(a_Ranges.size());
    for (const auto& range : a_Ranges) {
        VkMappedMemoryRange vkRange{ VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE };
        vkRange.memory = *range.memory;
        vkRange.offset = range.offset;
        vkRange.size   = range.length;
        vkRanges.push_back(vkRange);
    }
    vkFlushMappedMemoryRanges(device, vkRanges.size(), vkRanges.data());
}

}