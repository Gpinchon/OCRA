/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/

//See SwiftShader !
#include <Command/Buffer.hpp>

#include <GL/Command/ExecutionState.hpp>

#include <functional>
#include <array>
#include <cassert>

namespace OCRA::Command::Buffer
{
enum class State {
	Initial, Recording, Executable, Pending, Invalid
};
struct Impl
{
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

Handle Create(const Device::Handle& a_Device) {
    return Handle(new Impl);
}

void PushCommand(const Handle& a_CommandBuffer, const CallBack& a_Callback) {
	a_CommandBuffer->PushCommand(a_Callback);
}
}

namespace OCRA::Command
{
void BeginCommandBuffer(const Buffer::Handle& a_CommandBuffer) {
	a_CommandBuffer->Begin();
}
void EndCommandBuffer(const Buffer::Handle& a_CommandBuffer) {
	a_CommandBuffer->End();
}

void ResetCommandBuffer(const Buffer::Handle& a_CommandBuffer) {
	a_CommandBuffer->Reset();
}
void Submit(const Buffer::Handle& a_CommandBuffer, bool a_Once) {
	a_CommandBuffer->Submit(a_Once);
}

void ExecuteCommands(
	const Buffer::Handle& a_CommandBuffer,
	const Buffer::Handle& a_SecondaryCommandBuffer)
{
	a_CommandBuffer->PushCommand([
		commandBuffer = a_SecondaryCommandBuffer
	](Buffer::ExecutionState& executionState) {
		commandBuffer->SubmitSecondary(executionState);
	});
}
}