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
    Handle          memory;     //the memory to map
    uint64_t        offset{ 0 };//the beginning index inside the memory
    uint64_t        length{ 0 };//the length of memory to map
};

/**
* @brief Allocates a buffer on device's memory
*/
Handle Allocate(const Device::Handle& a_Device, const Info& a_Info);

/**
* @brief Maps the device memory to client's memory
* @return a pointer to the memory
*/
void* Map(
    const Device::Handle&   a_Device,
    const MappedRange&      a_MemoryRange);

/**
* @brief Unmaps the specified memory, invalidating the pointer returning by Map
*/
void Unmap(
    const Device::Handle&  a_Device,
    const Handle&          a_Memory);

/**
* @brief Forces an update on the device's side of the memory's content
*/
void FlushMappedRanges(
    const Device::Handle&           a_Device,
    const std::vector<MappedRange>& a_Ranges);
}