/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/
#pragma once

#include <Handle.hpp>
#include <Scalar.hpp>
#include <IndexType.hpp>

namespace OCRA::Command
{
//Vertex buffer copy commands
void ReadVertexBufferFrom(
	const CommandBuffer::Handle& a_CommandBuffer,
	const Buffer::Vertex::Handle& a_DstVBO,
	const Buffer::Transfer::Handle& a_SrcTransferBuffer,
	Uint64 a_ReadOffset, Uint64 a_WriteOffset, Uint64 a_Size);
void WriteVertexBufferTo(
	const CommandBuffer::Handle& a_CommandBuffer,
	const Buffer::Vertex::Handle& a_SrcVBO,
	const Buffer::Transfer::Handle& a_DstTransferBuffer,
	Uint64 a_ReadOffset, Uint64 a_WriteOffset, Uint64 a_Size);
//Vertex buffer binding commands
void BindIndexBuffer(
	const CommandBuffer::Handle& a_CommandBuffer,
	const Buffer::Vertex::Handle& a_IndexBuffer,
	const Uint64 offset,
	const Buffer::Vertex::IndexType indexType);
}