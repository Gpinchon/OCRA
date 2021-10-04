/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:50
*/

#include <Handle.hpp>

#include <map>

#include <GL/Buffer/Buffer.hpp>
#include <GL/glew.h>

namespace OCRA::Buffer::Transfer
{
struct Impl : public Buffer::Impl {
	Impl(const Device::Handle& a_Device, const Info& a_Info)
	    : info(a_Info)
	{
		if (info.accessFlag == Read)
			accessFlags = GL_MAP_READ_BIT;
		else if (info.accessFlag == Write)
			accessFlags = GL_MAP_WRITE_BIT;
		else if (info.accessFlag == ReadWrite)
			accessFlags = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;
		glNamedBufferStorage(
			handle,
			info.size,
			nullptr,
			accessFlags);
	}
	const Info info;
	GLbitfield accessFlags{ 0 }:
};
static Handle s_CurrentHandle = 0;
static std::map<Handle, Impl> s_TransferBuffers;
Handle Create(const Device::Handle& a_Device, const Info& a_Info)
{
	++s_CurrentHandle;
    s_TransferBuffers.emplace(s_CurrentHandle, Impl(a_Device, a_Info));
    return s_CurrentHandle;
}
void Destroy(const Device::Handle& a_Device, const Handle& a_Handle)
{
	s_TransferBuffers.erase(s_CurrentHandle);
}
const Info& GetInfo(const Device::Handle& a_Device, const Handle& a_Handle)
{
    return s_TransferBuffers.at(a_Handle).info;
}
void* Map(const Device::Handle& a_Device, const Handle& a_Handle, Uint64 offset, Uint64 size)
{
	auto buffer{ s_TransferBuffers.at(a_Handle) };
	return glMapNamedBufferRange(
			buffer.handle,
			offset,
			size,
			buffer.accessFlags);
}
void Unmap(const Device::Handle& a_Device, const Handle& a_Handle)
{
	glUnmapNamedBuffer(s_TransferBuffers.at(a_Handle).handle);
}
}