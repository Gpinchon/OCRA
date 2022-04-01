#pragma once

#include <Handle.hpp>

#include <bitset>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::Memory);

namespace OCRA::Memory
{
struct Info {
    size_t      size{ 0 };
    uint32_t    memoryTypeIndex{ 0 };
};
Handle Allocate(const Device::Handle& a_Device, const Info& a_Info);
void* Map(
	const Device::Handle&  a_Device,
	const Handle&          a_Memory,
	const uint64_t&        a_Offset,
	const uint64_t&        a_Length);
void Unmap(
	const Device::Handle&  a_Device,
	const Handle&          a_Memory);
}