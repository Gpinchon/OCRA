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
#include <RenderPass.hpp>

#include <vector>

HANDLE(OCRA::Command::Buffer);

namespace OCRA::Command
{
struct RenderPassBeginInfo
{
	RenderPass::Handle renderPass{ 0 };
	FrameBuffer::Handle framebuffer{ 0 };
	Rect2D renderArea;
	std::vector<ClearValue> clearValues;
};
enum class SubPassContents
{
	Inline, SecondaryCommandBuffers
};
//Begin Render Pass recording
void BeginRenderPass(const Command::Buffer::Handle& a_CommandBuffer, const RenderPassBeginInfo& a_BeginInfo, const SubPassContents& a_SubPassContents);
//End Render Pass recording
void EndRenderPass(const Command::Buffer::Handle& a_CommandBuffer);
}