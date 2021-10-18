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

#include <vector>

namespace OCRA::Command::VertexBuffer
{
//Vertex buffer copy commands
void ReadFrom(
	const Command::Buffer::Handle& a_CommandBuffer,
	const OCRA::Buffer::Vertex::Handle& a_DstVBO,
	const OCRA::Buffer::Transfer::Handle& a_SrcTransferBuffer,
	Uint64 a_ReadOffset, Uint64 a_WriteOffset, Uint64 a_Size);
void WriteTo(
	const Command::Buffer::Handle& a_CommandBuffer,
	const OCRA::Buffer::Vertex::Handle& a_SrcVBO,
	const OCRA::Buffer::Transfer::Handle& a_DstTransferBuffer,
	Uint64 a_ReadOffset, Uint64 a_WriteOffset, Uint64 a_Size);
//Bind specified Vertex Buffer to Indices of Graphics Pipeline currently bound to this Command Buffer
void BindIndexBuffer(
	const Command::Buffer::Handle& a_CommandBuffer,
	const OCRA::Buffer::Vertex::Handle& a_IndexBuffer,
	const Uint64 offset,
	const IndexType indexType);
//Bind specified Vertex Buffers to Graphics Pipeline currently bound to this Command Buffer
void BindVertexBuffers(
	const Command::Buffer::Handle& a_CommandBuffer,
	const Uint32 firstBinding,
	const Uint32 bindingCount,
	const std::vector<OCRA::Buffer::Vertex::Handle>& a_VertexBuffers,
	const std::vector<Uint64> a_Offsets);
}