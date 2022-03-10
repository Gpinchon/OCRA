/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/
#pragma once

#include <Handle.hpp>
#include <IndexType.hpp>
#include <Command/Buffer.hpp>
#include <Buffer/Vertex.hpp>
#include <Buffer/Transfer.hpp>

#include <vector>

namespace OCRA::Command::VertexBuffer
{
//Vertex buffer copy commands
void ReadFrom(
	const Command::Buffer::Handle& a_CommandBuffer,
	const OCRA::Buffer::Vertex::Handle& a_DstVBO,
	const OCRA::Buffer::Transfer::Handle& a_SrcTransferBuffer,
	uint64_t a_ReadOffset, uint64_t a_WriteOffset, uint64_t a_Size);
void WriteTo(
	const Command::Buffer::Handle& a_CommandBuffer,
	const OCRA::Buffer::Vertex::Handle& a_SrcVBO,
	const OCRA::Buffer::Transfer::Handle& a_DstTransferBuffer,
	uint64_t a_ReadOffset, uint64_t a_WriteOffset, uint64_t a_Size);
}