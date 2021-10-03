/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:17
*/

#include <Scalar.hpp>
#include <Vertex/Buffer.hpp>

#include <map>

#include <GL/glew.h>

namespace OCRA::Vertex::Buffer {
struct Impl {
	Impl(const Device::Handle& a_Device, const Info& a_Info)
		: info(a_Info)
	{
		glCreateBuffers(1, &handle);
		glNamedBufferStorage(handle, info.size, nullptr, 0);
	}
	~Impl() {
		glDeleteBuffers(1, &handle);
	}
	const Info info;
	GLuint handle{ 0 };
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
unsigned GetGLHandle(const Device::Handle& a_Device, const Handle& a_Handle)
{
	return s_VertexBuffers.at(a_Handle).handle;
}
}