/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:17
*/
#pragma once

#include <Handle.hpp>
#include <Scalar.hpp>
#include <Buffer/Range.hpp>

#include <GL/glew.h>

namespace OCRA::Buffer {
typedef HandleType Handle;
struct Info
{
	Uint64 size{ 0 };
	GLbitfield flags{ 0 }; //flags of the buffer storage
};
struct CopyOperation
{
	Buffer::Handle srcBuffer{ 0 }, dstBuffer{ 0 };
	Uint64 readOffset{ 0 }, writeOffset{ 0 }, size{ 0 };
}
struct MapOperation
{
	Buffer::Handle buffer{ 0 };
	Range range;
	GLbitfield flags;
}
struct FlushOperation
{
	Buffer::Handle buffer{ 0 };
	Range range;
}
Handle Create(const Device::Handle& a_Device, const Info& a_Info);
void Destroy(const Device::Handle& a_Device, const Handle& a_Handle);
const Info& GetInfo(const Device::Handle& a_Device, const Handle& a_Handle);
unsigned GetGLHandle(const Device::Handle& a_Device, const Handle& a_Handle);
void* Map(const Device::Handle& a_Device, const MapOperation& a_MapOperation);
void Flush(const Device::Handle& a_Device, const FlushOperation& a_FlushOperation);
void Unmap(const Device::Handle& a_Device, const Handle& a_Handle);
void Copy(const Device::Handle& a_Device, const CopyOperation& a_CopyOperation);
}