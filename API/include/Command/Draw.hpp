/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/
#pragma once

#include <Handle.hpp>

OCRA_DECLARE_HANDLE(OCRA::Command::Buffer);

namespace OCRA::Command
{
//Draw commands
void Draw(
	const Command::Buffer::Handle& a_CommandBuffer,
	const uint32_t a_VertexCount,
	const uint32_t a_InstanceCount,
	const uint32_t a_FirstVertex,
	const uint32_t a_FirstInstance);
void DrawIndexed(
	const Command::Buffer::Handle& a_CommandBuffer,
	const uint32_t a_IndexCount,
    const uint32_t a_InstanceCount,
    const uint32_t a_FirstIndex,
    const uint32_t a_VertexOffset,
    const uint32_t a_FirstInstance);
}