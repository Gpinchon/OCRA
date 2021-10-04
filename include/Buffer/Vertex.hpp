/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:17
*/
#pragma once

#include <Handle.hpp>
#include <Scalar.hpp>

namespace OCRA::Buffer::Vertex {
struct Info {
	Uint64 size;
};
void ReadFrom(const Device::Handle& a_Device, const Handle& a_Handle, const Buffer::Transfer::Handle& a_Buffer, Uint64 a_ReadOffset, Uint64 a_WriteOffset, Uint64 a_Size);
void WriteTo(const Device::Handle& a_Device, const Handle& a_Handle, const Buffer::Transfer::Handle& a_Buffer, Uint64 a_ReadOffset, Uint64 a_WriteOffset, Uint64 a_Size);
}