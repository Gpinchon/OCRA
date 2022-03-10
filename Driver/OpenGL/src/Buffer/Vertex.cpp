/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:17
*/

#include <Buffer/Vertex.hpp>
#include <Buffer/Transfer.hpp>

#include <GL/Buffer/Vertex.hpp>
#include <GL/Buffer/Buffer.hpp>
#include <GL/Buffer/Transfer.hpp>
#include <GL/glew.h>

namespace OCRA::Buffer::Vertex {
struct Impl
{
	Impl(const Device::Handle& a_Device, const Info& a_Info)
		: device(a_Device)
		, info(a_Info)
	{
		Buffer::Info bufferInfo;
		bufferInfo.size = a_Info.size;
		bufferInfo.flags = 0; //vertex buffer is pure GPU memory
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
void ReadFrom(
	const Device::Handle& a_Device,
	const Buffer::Vertex::Handle& a_DstVBO,
	const Buffer::Transfer::Handle& a_SrcTransferBuffer,
	uint64_t a_ReadOffset, uint64_t a_WriteOffset, uint64_t a_Size)
{
	CopyOperation copy;
	copy.srcBuffer = Buffer::Transfer::GetBufferHandle(a_SrcTransferBuffer);
	copy.dstBuffer = Buffer::Vertex::GetBufferHandle(a_DstVBO);
	copy.readOffset = a_ReadOffset;
	copy.writeOffset = a_WriteOffset;
	copy.size = a_WriteOffset;
	Buffer::Copy(a_Device, copy);
}
void WriteTo(
	const Device::Handle& a_Device,
	const Buffer::Vertex::Handle& a_SrcVBO,
	const Buffer::Transfer::Handle& a_DstTransferBuffer,
	uint64_t a_ReadOffset, uint64_t a_WriteOffset, uint64_t a_Size)
{
	CopyOperation copy;
	copy.srcBuffer = Buffer::Vertex::GetBufferHandle(a_SrcVBO);
	copy.dstBuffer = Buffer::Transfer::GetBufferHandle(a_DstTransferBuffer);
	copy.readOffset = a_ReadOffset;
	copy.writeOffset = a_WriteOffset;
	copy.size = a_WriteOffset;
	Buffer::Copy(a_Device, copy);
}

}