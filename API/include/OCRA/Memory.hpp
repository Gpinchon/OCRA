#pragma once

#include <OCRA/Handle.hpp>

#include <bitset>
#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::Memory);

namespace OCRA::Memory
{
struct Info {
    size_t      size{ 0 };
    uint32_t    memoryTypeIndex{ 0 };
};
struct MappedRange {
    Handle          memory;
    uint64_t        offset{ 0 };
    uint64_t        length{ 0 };
};
Handle Allocate(const Device::Handle& a_Device, const Info& a_Info);
void* Map(
    const Device::Handle&   a_Device,
    const MappedRange&      a_MemoryRange);
void Unmap(
    const Device::Handle&  a_Device,
    const Handle&          a_Memory);
void FlushMappedRanges(
    const Device::Handle&           a_Device,
    const std::vector<MappedRange>& a_Ranges);
}