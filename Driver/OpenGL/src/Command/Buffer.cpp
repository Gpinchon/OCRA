/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/
#pragma once

#include <Handle.hpp>
#include <Command/RenderPass.hpp>

#include <functional>
#include <array>

namespace OCRA::Command::Buffer
{
typedef CallBack std::function<void()>;
struct Impl
{
	void Reset()
	{
		commandsCount = 0;
	}
	void StartRecording()
	{
		Reset();
		recording = true;
	}
	PushCommand(const CallBack& a_Command)
	{
		commands.at(commandsCount) = a_Command;
		++commandsCount;
	}
	void EndRecording()
	{
		recording = false;
	}
	bool recording{ false };
	Uint32 commandsCount{ 0 };
	std::array<CallBack, 1024> commands;
}
struct TemporaryState
{
	
}
static Handle s_CurrentHandle = 0;
static std::map<Handle, Impl> s_Buffers;
Handle Create(const Device::Handle& a_Device)
{
	++s_CurrentHandle;
    s_Buffers.emplace(s_CurrentHandle, Impl());
    return s_CurrentHandle;
}
auto& Get(const Handle& a_CommandBuffer)
{
	return s_Buffers.at(a_CommandBuffer);
}
void Destroy(const Device::Handle& a_Device, const Handle& a_CommandBuffer)
{
	s_Buffers.erase(a_CommandBuffer);
}
void Begin(const Handle& a_CommandBuffer)
{
	Get(a_CommandBuffer).StartRecording();
}
void PushCommand(const Handle& a_CommandBuffer, const CallBack& a_Command)
{
	Get(a_CommandBuffer).PushCommand(a_Command);
}
void End(const Handle& a_CommandBuffer)
{
	Get(a_CommandBuffer).EndRecording();
}
}

#include <RenderPass.hpp>
#include <FrameBuffer.hpp>

namespace OCRA::Command::RenderPass
{
//Begin Render Pass recording
void Begin(const Command::Buffer::Handle& a_CommandBuffer, const BeginInfo& a_BeginInfo, const SubPassContents& a_SubPassContents)
{
	auto renderPassInfo{ RenderPass::GetInfo(a_BeginInfo.renderPass) };
	auto frameBufferInfo{ FrameBuffer::GetInfo(a_BeginInfo.framebuffer) };
	
	Buffer::Get(a_CommandBuffer).renderPass = a_BeginInfo;
}
//End Render Pass recording
void End(const Command::Buffer::Handle& a_CommandBuffer)
{
	
}
}