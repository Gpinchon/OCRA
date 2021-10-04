/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:17
*/

#include <Scalar.hpp>
#include <Buffer/Vertex.hpp>

#include <map>

#include <GL/Buffer/Buffer.hpp>
#include <GL/Buffer/Transfer.hpp>
#include <GL/glew.h>

namespace OCRA::Buffer::Vertex {
struct Impl : public Buffer::Impl {
	Impl(const Device::Handle& a_Device, const Info& a_Info)
		: info(a_Info)
	{
		glNamedBufferStorage(handle, info.size, nullptr, 0);
	}
	const Info info;
};
static Handle s_CurrentHandle = 0;
static std::map<Handle, Impl> s_VertexBuffers;
Handle Create(const Device::Handle& a_Device, const Info& a_Info)
{
	++s_CurrentHandle;
	s_VertexBuffers.emplace(s_CurrentHandle, Impl(a_Device, a_Info));
	return s_CurrentHandle;
}
void Destroy(const Device::Handle& a_Device, const Handle& a_Handle)
{
	s_VertexBuffers.erase(a_Handle);
}
const Info& GetInfo(const Device::Handle& a_Device, const Handle& a_Handle)
{
	return s_VertexBuffers.at(a_Handle).info;
}
void ReadFrom(const Device::Handle& a_Device, const Handle& a_Handle, const Buffer::Transfer::Handle& a_Buffer, Uint64 a_ReadOffset, Uint64 a_WriteOffset, Uint64 a_Size)
{
	glCopyNamedBufferSubData(
		Buffer::Transfer::GetGLHandle(a_Device, a_Buffer),
		Buffer::Vertex::GetGLHandle(a_Device, a_Handle),
		a_ReadOffset, a_WriteOffset, a_Size);
}
void WriteTo(const Device::Handle& a_Device, const Handle& a_Handle, const Buffer::Transfer::Handle& a_Buffer, Uint64 a_ReadOffset, Uint64 a_WriteOffset, Uint64 a_Size)
{
	glCopyNamedBufferSubData(
		Buffer::Vertex::GetGLHandle(a_Device, a_Handle),
		Buffer::Transfer::GetGLHandle(a_Device, a_Buffer),
		a_ReadOffset, a_WriteOffset, a_Size);
}
unsigned GetGLHandle(const Device::Handle& a_Device, const Handle& a_Handle)
{
	return s_VertexBuffers.at(a_Handle).handle;
}
}