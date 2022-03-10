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
		GLbitfield accessFlags = 0;
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
	const Device::Handle device;
	Buffer::Handle bufferHandle;
	const Info info;
};
Handle Create(const Device::Handle& a_Device, const Info& a_Info)
{
	return Handle(new Impl(a_Device, a_Info));
}
const Info& GetInfo(const Device::Handle& a_Device, const Handle& a_Handle)
{
	return a_Handle->info;
}
Buffer::Handle GetBufferHandle(const Handle& a_Handle)
{
	return a_Handle->bufferHandle;
}
void* Map(const Device::Handle& a_Device, const MapOperation& a_MapOperation)
{
	Buffer::MapOperation map;
	map.buffer = GetBufferHandle(a_MapOperation.buffer);
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
	flush.buffer = GetBufferHandle(a_FlushOperation.buffer);
	flush.range = a_FlushOperation.range;
	return Buffer::Flush(a_Device, flush);
}
void Unmap(const Device::Handle& a_Device, const Handle& a_Handle)
{
	return Buffer::Unmap(a_Device, GetBufferHandle(a_Handle));
}
}