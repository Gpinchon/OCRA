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
#include <Command/RenderPass.hpp>
#include <Pipeline/Pipeline.hpp>
#include <RenderPass.hpp>
#include <Buffer/Vertex.hpp>

HANDLE(OCRA::Command::Buffer);

//Types definition
namespace OCRA::Command
{
struct RenderPassBeginInfo
{
	RenderPass::Handle renderPass{ 0 };
	FrameBuffer::Handle framebuffer{ 0 };
	Rect2D renderArea;
	uint8_t clearValueCount{ 0 };
	std::array<ClearValue, FrameBuffer::MaxColorAttachments> clearValues;
};
enum class SubPassContents
{
	Inline, SecondaryCommandBuffers
};
}

//Functions declaration
namespace OCRA::Command::Buffer
{
struct SubmitInfo {
Handle commandBuffer;
uint32_t deviceMask;//TODO : ???
};
//Command Buffer creation
Handle Create(const Device::Handle& a_Device);

//Begin Command Buffer recording, swutcgubg it to Recording state
void Begin(const Handle& a_CommandBuffer);
//End Command Buffer recording, switching it to Executable state
void End(const Handle& a_CommandBuffer);

//Reset Command Buffer to Initial state
void Reset(const Handle& a_CommandBuffer);

//Begin Render Pass recording
void BeginRenderPass(const Handle& a_CommandBuffer, const RenderPassBeginInfo& a_BeginInfo, const SubPassContents& a_SubPassContents);
//End Render Pass recording
void EndRenderPass(const Handle& a_CommandBuffer);

void ExecuteCommands(
	const Buffer::Handle& a_CommandBuffer,
	const Buffer::Handle& a_SecondaryCommandBuffer);

//Bind the specified Pipeline to the specified Command Buffer at the specified Binding Point
void BindPipeline(
	const Handle& a_CommandBuffer,
	const Pipeline::BindingPoint& a_BindingPoint,
	const Pipeline::Handle& a_Pipeline);

//Bind specified Vertex Buffer to Render Pass currently bound to this Command Buffer
void BindIndexBuffer(
	const Handle& a_CommandBuffer,
	const OCRA::Buffer::Vertex::Handle& a_IndexBuffer,
	const uint64_t offset,
	const IndexType indexType);
//Bind specified Vertex Buffers to Render Pass currently bound to this Command Buffer
void BindVertexBuffers(
	const Handle& a_CommandBuffer,
	const uint32_t firstBinding,
	const uint32_t bindingCount,
	const std::vector<OCRA::Buffer::Vertex::Handle>& a_VertexBuffers,
	const std::vector<uint64_t>& a_Offsets);
}