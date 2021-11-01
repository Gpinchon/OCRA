/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/

/////TODO : REDO THIS SHIT !!!

#include <Handle.hpp>
#include <Command/RenderPass.hpp>
#include <RenderPass.hpp>
#include <FrameBuffer.hpp>

#include <GL/IndexType.hpp>
#include <GL/RenderPass.hpp>

#include <functional>
#include <array>
#include <mutex>

namespace OCRA::Command::Buffer
{
typedef CallBack std::function<void()>;
struct Impl
{
	void Reset()
	{
		commandsCount = 0;
		renderPass = {};
	}
	void StartRecording()
	{
		Reset();
		recording.lock();
	}
	void PushCommand(const CallBack& a_Command)
	{
		commands.at(commandsCount) = a_Command;
		++commandsCount;
	}
	void EndRecording()
	{
		recording.unlock();
	}
	Uint32 commandsCount{ 0 };
	std::array<CallBack, 1024> commands;
	std::map<RenderPass::Handle, RenderPass> renderPassList;
	RenderPass::Handle currentRenderPass{ 0 };
	std::mutex recording;
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

namespace OCRA::Command {
//Begin Render Pass recording
void BeginRenderPass(const Command::Buffer::Handle& a_CommandBuffer, const RenderPassBeginInfo& a_BeginInfo, const SubPassContents& a_SubPassContents)
{
	auto& commandBuffer{ Buffer::Get(a_CommandBuffer) };
	auto& commandBufferRP{ commandBuffer.renderPassList[a_BeginInfo.renderPass] };
	commandBuffer.currentRenderPass = a_BeginInfo.renderPass;
	commandBufferRP.StartRecording(a_BeginInfo);
}
//End Render Pass recording
void EndRenderPass(const Command::Buffer::Handle& a_CommandBuffer)
{
	auto& commandBuffer{ Buffer::Get(a_CommandBuffer) };
	auto& commandBufferRP{ commandBuffer.renderPassList[a_BeginInfo.renderPass] };
	commandBuffer.currentRenderPass = 0;
	commandBufferRP.EndRecording();
}

//Bind specified Vertex Buffer to Render Pass currently bound to this Command Buffer
void BindIndexBuffer(
	const Command::Buffer::Handle& a_CommandBuffer,
	const OCRA::Buffer::Vertex::Handle& a_IndexBuffer,
	const Uint64 a_Offset,
	const IndexType a_IndexType)
{
	auto& commandBuffer{ Buffer::Get(a_CommandBuffer) };
	auto& commandBufferRP{ commandBuffer.renderPassList[commandBuffer.currentRenderPass] };
	const auto& bufferHandle{ Buffer::Vertex::GetBufferHandle(a_IndexBuffer) };
	commandBufferRP.indexBuffer.buffer = Buffer::GetGLHandle(bufferHandle);
	commandBufferRP.indexBuffer.offset = offset;
	commandBufferRP.indexBuffer.indexType = GetGLIndexType(a_IndexType);
}
//Bind specified Vertex Buffers to Render Pass currently bound to this Command Buffer
void BindVertexBuffers(
	const Command::Buffer::Handle& a_CommandBuffer,
	const Uint32 firstBinding,
	const Uint32 bindingCount,
	const std::vector<OCRA::Buffer::Vertex::Handle>& a_VertexBuffers,
	const std::vector<Uint64>& a_Offsets)
{
	auto& commandBuffer{ Buffer::Get(a_CommandBuffer) };
	auto& commandBufferRP{ commandBuffer.renderPassList[commandBuffer.currentRenderPass] };
	commandBufferRP.vertexBinding.firstBinding = firstBinding;
	commandBufferRP.vertexBinding.bindingCount = bindingCount;
	for (auto index = 0u; index < bindingCount; ++index)
	{
		const auto& bufferHandle{ Buffer::Vertex::GetBufferHandle(a_VertexBuffers.at(index)) };
		commandBufferRP.vertexBinding.vertexBuffers.at(index) = Buffer::GetGLHandle(bufferHandle);
		commandBufferRP.vertexBinding.offsets.at(index) = a_Offsets.at(index);
	}
}
}
