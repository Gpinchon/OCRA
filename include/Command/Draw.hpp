/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/
#pragma once

#include <Handle.hpp>
#include <Scalar.hpp>

namespace OCRA::Command
{
//Draw commands
void Draw(
	const CommandBuffer::Handle& a_CommandBuffer,
	const Uint32 a_VertexCount,
	const Uint32 a_InstanceCount,
	const Uint32 a_FirstVertex,
	const Uint32 a_FirstInstance);
void DrawIndexed(
	const CommandBuffer::Handle& a_CommandBuffer,
	const Uint32 a_IndexCount,
	const Uint32 a_InstanceCount,
	const Uint32 a_VertexOffset,
	const Uint32 a_FirstInstance);
}