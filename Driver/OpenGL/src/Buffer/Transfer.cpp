/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:50
*/

#include <Handle.hpp>
#include <Buffer/Transfer.hpp>

#include <map>

#include <GL/Buffer/Buffer.hpp>
#include <GL/glew.h>

namespace OCRA::Buffer::Transfer
{
struct Impl
{
	Impl(const Device::Handle& a_Device, const Info& a_Info)
		: device(a_Device)
		, info(a_Info)
	{
		GLbitfield accessFlags;
		if (info.accessFlag == Read)
			accessFlags = GL_MAP_READ_BIT;
		else if (info.accessFlag == Write)
			accessFlags = GL_MAP_WRITE_BIT;
		else if (info.accessFlag == ReadWrite)
			accessFlags = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;
		Buffer::Info bufferInfo;
		bufferInfo.size = a_Info.size;
		bufferInfo.flags = accessFlags;
		bufferHandle = Buffer::Create(a_Device, bufferInfo);
	}
	~Impl()
	{
		Buffer::Destroy(device, bufferHandle);
	}
	Device::Handle device;
	Buffer::Handle bufferHandle;
	const Info info;
};
static Handle s_CurrentHandle = 0;
static std::map<Handle, Info> s_TransferBuffers;
Handle Create(const Device::Handle& a_Device, const Info& a_Info)
{
	++s_CurrentHandle;
	s_TransferBuffers.emplace(s_CurrentHandle, { a_Device, a_Info };
	return s_CurrentHandle;
}
Handle Destroy(const Device::Handle& a_Device, const Handle& a_Handle)
{
	s_TransferBuffers.erase(a_Handle);
}
const Info& GetInfo(const Device::Handle& a_Device, const Handle& a_Handle)
{
	return s_TransferBuffers.at(a_Handle);
}
Buffer::Handle GetBufferHandle(const Device::Handle& a_Device, const Handle& a_Handle)
{
	return s_VertexBuffers.at(a_Handle).bufferHandle;
}
void* Map(const Device::Handle& a_Device, const MapOperation& a_MapOperation)
{
	Buffer::MapOperation map;
	map.buffer = GetBufferHandle(a_Device, a_MapOperation.buffer);
	map.range = a_MapOperation.range;
	if ((a_MapOperation.flags & Read) == Read)
		map.flags |= GL_MAP_READ_BIT;
	if ((a_MapOperation.flags & Write) == Write)
		map.flags |= GL_MAP_WRITE_BIT;
	return Buffer::Map(a_Device, map);
}
void Flush(const Device::Handle& a_Device, const FlushOperation& a_FlushOperation)
{
	Buffer::FlushOperation flush;
	flush.buffer = GetBufferHandle(a_Device, a_FlushOperation.buffer);
	flush.range = a_FlushOperation.range;
	return Buffer::Flush(a_Device, flush);
}
void Unmap(const Device::Handle& a_Device, const Handle& a_Handle)
{
	return Buffer::Unmap(a_Device, GetBufferHandle(a_Device, a_Handle));
}
}