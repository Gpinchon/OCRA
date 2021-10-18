/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/
#pragma once

#include <Handle.hpp>
#include <Rect2D.hpp>
#include <FrameBuffer.hpp>
#include <ClearValue.hpp>

namespace OCRA::Command::RenderPass
{
struct BeginInfo
{
	RenderPass::Handle renderPass{ 0 };
	FrameBuffer::Handle framebuffer{ 0 };
	Rect2D renderArea;
	Uint8 clearValueCount{ 0 };
	std::array<ClearValue, Framebuffer::MaxColorAttachments> clearValues;
};
enum class SubPassContents
{
	Inline, SecondaryCommandBuffers
};
//Begin Render Pass recording
void Begin(const Command::Buffer::Handle& a_CommandBuffer, const BeginInfo& a_BeginInfo, const SubPassContents& a_SubPassContents);
//End Render Pass recording
void End(const Command::Buffer::Handle& a_CommandBuffer);
}