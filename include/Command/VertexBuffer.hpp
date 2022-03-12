/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/
#pragma once

#include <IndexType.hpp>
#include <Command/Buffer.hpp>
#include <Buffer/Vertex.hpp>
#include <Buffer/Transfer.hpp>

#include <vector>

namespace OCRA::Command
{
//Vertex buffer copy commands
void ReadVertexBufferFrom(
	const Command::Buffer::Handle& a_CommandBuffer,
	const Buffer::Vertex::Handle& a_DstVBO,
	const Buffer::Transfer::Handle& a_SrcTransferBuffer,
	uint64_t a_ReadOffset, uint64_t a_WriteOffset, uint64_t a_Size);
void WriteVertexBufferTo(
	const Command::Buffer::Handle& a_CommandBuffer,
	const Buffer::Vertex::Handle& a_SrcVBO,
	const Buffer::Transfer::Handle& a_DstTransferBuffer,
	uint64_t a_ReadOffset, uint64_t a_WriteOffset, uint64_t a_Size);
//Bind specified Vertex Buffers to Render Pass currently bound to this Command Buffer
void BindVertexBuffers(
	const Command::Buffer::Handle& a_CommandBuffer,
	const uint32_t firstBinding,
	const uint32_t bindingCount,
	const std::vector<Buffer::Vertex::Handle>& a_VertexBuffers,
	const std::vector<uint64_t>& a_Offsets);
//Bind specified Vertex Buffer to Render Pass currently bound to this Command Buffer
void BindIndexBuffer(
	const Command::Buffer::Handle& a_CommandBuffer,
	const Buffer::Vertex::Handle& a_IndexBuffer,
	const uint64_t offset,
	const IndexType indexType);
}