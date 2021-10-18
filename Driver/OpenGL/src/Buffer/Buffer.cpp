/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:50
*/

#include <GL/Buffer/Buffer.hpp>

#include <cassert>

namespace OCRA::Buffer {
struct Impl
{
	Impl(const Device::Handle& a_Device, const Info& a_Info)
		: info(a_Info)
	{
		glCreateBuffers(1, &handle);
		glNamedBufferStorage(
			handle,
			info.size,
			nullptr,
			info.flags);
	}
	~Impl()
	{
		glDeleteBuffers(1, &handle);
	}
	GLuint handle{ 0 };
	const Info info;
};
static Handle s_CurrentHandle = 0;
static std::map<Handle, Impl> s_Buffers;
Handle Create(const Device::Handle& a_Device, const Info& a_Info)
{
	++s_CurrentHandle;
    s_Buffers.emplace(s_CurrentHandle, { a_Device, a_Info });
    return s_CurrentHandle;
}
void Destroy(const Device::Handle& a_Device, const Handle& a_Handle)
{
	s_Buffers.erase(a_Handle);
}
const Info& GetInfo(const Device::Handle& a_Device, const Handle& a_Handle)
{
    return s_Buffers.at(a_Handle).info;
}
unsigned GetGLHandle(const Handle& a_Handle)
{
	return s_Buffers.at(a_Handle).handle;
}
void* Map(const Device::Handle& a_Device, const MapOperation& a_MapOperation)
{
	return glMapNamedBufferRange(
			GetGLHandle(a_Device, a_MapOperation.buffer),
			a_MapOperation.range.offset,
			a_MapOperation.range.size,
			a_MapOperation.flags); 
}
void Flush(const Device::Handle& a_Device, const FlushOperation& a_FlushOperation)
{
	glFlushMappedNamedBufferRange(
		GetGLHandle(a_Device, a_FlushOperation.buffer),
		a_FlushOperation.range.offset,
		a_FlushOperation.range.size);
}
void Unmap(const Device::Handle& a_Device, const Handle& a_Handle)
{
	glUnmapNamedBuffer(GetGLHandle(a_Device, a_Handle));
}
void Copy(const Device::Handle& a_Device, const CopyOperation& a_CopyOperation)
{
	glCopyNamedBufferSubData(
		GetGLHandle(a_Device, a_CopyOperation.srcBuffer),
		GetGLHandle(a_Device, a_CopyOperation.dstBuffer),
		a_CopyOperation.readOffset, a_CopyOperation.writeOffset, a_CopyOperation.size);
}
}