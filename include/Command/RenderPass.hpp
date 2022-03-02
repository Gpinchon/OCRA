/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/
#pragma once

#include <Handle.hpp>
#include <Common/Rect2D.hpp>
#include <FrameBuffer.hpp>
#include <ClearValue.hpp>
#include <Pipeline/BindingPoint.hpp>
#include <IndexType.hpp>

#include <array>
#include <vector>

namespace OCRA::Command
{
struct RenderPassBeginInfo
{
	OCRA::RenderPass::Handle renderPass{ 0 };
	FrameBuffer::Handle framebuffer{ 0 };
	Rect2D renderArea;
	Uint8 clearValueCount{ 0 };
	std::array<ClearValue, OCRA::FrameBuffer::MaxColorAttachments> clearValues;
};
enum class SubPassContents
{
	Inline, SecondaryCommandBuffers
};
//Begin Render Pass recording
void BeginRenderPass(const Command::Buffer::Handle& a_CommandBuffer, const RenderPassBeginInfo& a_BeginInfo, const SubPassContents& a_SubPassContents);
//End Render Pass recording
void EndRenderPass(const Command::Buffer::Handle& a_CommandBuffer);
//Bind the specified Pipeline to the specified Command Buffer at the specified Binding Point
void BindPipeline(
	const Command::Buffer::Handle& a_CommandBuffer,
	const Pipeline::BindingPoint& a_BindingPoint,
	const Pipeline::Handle& a_Pipeline);
//Bind specified Vertex Buffer to Render Pass currently bound to this Command Buffer
void BindIndexBuffer(
	const Command::Buffer::Handle& a_CommandBuffer,
	const OCRA::Buffer::Vertex::Handle& a_IndexBuffer,
	const Uint64 offset,
	const IndexType indexType);
//Bind specified Vertex Buffers to Render Pass currently bound to this Command Buffer
void BindVertexBuffers(
	const Command::Buffer::Handle& a_CommandBuffer,
	const Uint32 firstBinding,
	const Uint32 bindingCount,
	const std::vector<OCRA::Buffer::Vertex::Handle>& a_VertexBuffers,
	const std::vector<Uint64>& a_Offsets);
}