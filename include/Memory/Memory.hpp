/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:27:24
*/
#pragma once

#include <Handle.hpp>

namespace OCRA::Memory {
struct Info {
	uint64_t size{ 0 };
	uint32_t memoryTypeIndex{ 0 };
};
Handle Allocate(const Device::Handle& a_Device, const Info& a_Info);
Handle Free(const Device::Handle& a_Device, const Handle& a_Handle);
}