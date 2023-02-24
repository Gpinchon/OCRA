#pragma once

#include <OCRA/Handles.hpp>
#include <OCRA/Structs.hpp>

#include <vector>

namespace OCRA::Memory
{
/**
* @brief Maps the device memory to client's memory
* @return a pointer to the memory
*/
void* Map(const MemoryMappedRange& a_MemoryRange);

/**
* @brief Unmaps the specified memory, invalidating the pointer returning by Map
*/
void Unmap(const Handle& a_Memory);

/**
* @brief Forces an update on the device's side of the memory's content
*/
void FlushMappedRanges(const std::vector<MemoryMappedRange>& a_Ranges);
}