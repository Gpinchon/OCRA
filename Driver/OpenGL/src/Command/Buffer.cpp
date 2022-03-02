/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/

/////TODO : REDO THIS SHIT !!!
//See SwiftShader !

#include <Handle.hpp>
#include <Command/RenderPass.hpp>
#include <RenderPass.hpp>
#include <FrameBuffer.hpp>
#include <Pipeline/BindingPoint.hpp>

#include <GL/IndexType.hpp>
#include <GL/IndexType.hpp>
#include <GL/Buffer/Vertex.hpp>
#include <GL/Buffer/Buffer.hpp>
#include <GL/Command/ExecutionState.hpp>

#include <functional>
#include <array>
#include <mutex>
#include <cassert>
#include <map>

namespace OCRA::Command::Buffer
{
enum class State {
	Initial, Recording, Executable, Pending, Invalid
};
struct Impl
{
	Impl() {};
	
	typedef std::function<void(Impl::ExecutionState&)> CallBack;
	void Reset()
	{
		assert(
			state == State::Recording ||
			state == State::Executable ||
			state == State::Invalid
		);
		state = State::Initial;
		commands.clear();
		executionState = {};
	}
	void Invalidate()
	{
		assert(
			state == State::Recording ||
			state == State::Executable ||
			state == State::Pending
		);
		state = State::Invalid;
	}
	void Begin()
	{
		assert(state == State::Initial);
		state = State::Recording;
	}
	void End()
	{
		assert(state == State::Recording);
		state = State::Executable;
	}
	void Submit(bool a_Once)
	{
		assert(state == State::Executable);
		state = State::Pending;
		Execute(executionState);
		if (a_Once) Invalidate();
	}
	void SubmitSecondary(ExecutionState& a_ExecutionState)
	{
		assert(state == State::Executable);
		state = State::Pending;
		Execute(a_ExecutionState);
	}
	void Execute(ExecutionState& a_ExecutionState)
	{
		assert(state == State::Pending);
		for (const auto& command : commands)
			command(a_ExecutionState);
		state = State::Executable;
	}
	void PushCommand(const CallBack& a_Command)
	{
		assert(state == State::Recording);
		commands.push_back(a_Command);
	}
	State state{ State::Initial };
	std::vector<CallBack> commands;
	ExecutionState executionState{};
};
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
	Get(a_CommandBuffer).Begin();
}
void PushCommand(const Handle& a_CommandBuffer, const Impl::CallBack& a_Command)
{
	Get(a_CommandBuffer).PushCommand(a_Command);
}
void End(const Handle& a_CommandBuffer)
{
	Get(a_CommandBuffer).End();
}
void Submit(const Handle& a_CommandBuffer, bool a_Once)
{
	Get(a_CommandBuffer).Submit(a_Once);
}
}

namespace OCRA::Command {
//Begin Render Pass recording
void BeginRenderPass(const Command::Buffer::Handle& a_CommandBuffer, const RenderPassBeginInfo& a_BeginInfo, const SubPassContents& a_SubPassContents)
{
	Buffer::Get(a_CommandBuffer).PushCommand([beginInfo = a_BeginInfo](Buffer::Impl::ExecutionState& executionState){
		executionState.renderPass.beginInfo = beginInfo;
	});
}
//End Render Pass recording
void EndRenderPass(const Command::Buffer::Handle& a_CommandBuffer)
{
	Buffer::Get(a_CommandBuffer).PushCommand([](Buffer::Impl::ExecutionState& executionState){
		executionState.renderPass = {};
		executionState.subpassIndex = 0;
	});
}
void ExecuteCommands(
	const Buffer::Handle& a_CommandBuffer,
	const Buffer::Handle& a_SecondaryCommandBuffer)
{
	Buffer::Get(a_CommandBuffer).PushCommand([
		commandBuffer = a_SecondaryCommandBuffer
	](Buffer::Impl::ExecutionState& executionState) {
		Buffer::Get(commandBuffer).SubmitSecondary(executionState);
	});
}
void BindPipeline(
	const Buffer::Handle& a_CommandBuffer,
	const Pipeline::BindingPoint& a_BindingPoint,
	const Pipeline::Handle& a_Pipeline)
{
	Buffer::Get(a_CommandBuffer).PushCommand([
		bindingPoint = size_t(a_BindingPoint),
		pipeline = a_Pipeline
	](Buffer::Impl::ExecutionState& executionState){
		executionState.pipelineState.at(bindingPoint) = pipeline;
	});
}
//Bind specified Vertex Buffer to Render Pass currently bound to this Command Buffer
void BindIndexBuffer(
	const Command::Buffer::Handle& a_CommandBuffer,
	const OCRA::Buffer::Vertex::Handle& a_IndexBuffer,
	const Uint64 a_Offset,
	const IndexType a_IndexType)
{
	auto& commandBuffer{ Buffer::Get(a_CommandBuffer) };
	const auto& bufferHandle{ OCRA::Buffer::Vertex::GetBufferHandle(a_IndexBuffer) };
	commandBuffer.PushCommand([
		buffer = OCRA::Buffer::GetGLHandle(bufferHandle),
		offset = a_Offset,
		type = GetGLIndexType(a_IndexType)
	](Buffer::Impl::ExecutionState& executionState){
		executionState.renderPass.indexBufferBinding = { buffer, offset, type };
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	});
	
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
	for (auto index = 0u; index < bindingCount; ++index)
	{
		const auto& bufferHandle{ OCRA::Buffer::Vertex::GetBufferHandle(a_VertexBuffers.at(index)) };
		commandBuffer.PushCommand([
			binding = firstBinding + index,
			buffer = OCRA::Buffer::GetGLHandle(bufferHandle),
			offset = a_Offsets.at(index)
		](Buffer::Impl::ExecutionState& executionState){
			executionState.renderPass.vertexInputBindings.at(binding) = {buffer, offset};
		});
	}
}
}