/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/

#include <Command/Buffer.hpp>
#include <Command/Pool.hpp>
#include <Queue/Queue.hpp>

#include <GL/WeakHandle.hpp>
#include <GL/Command/Buffer.hpp>
#include <GL/Command/ExecutionState.hpp>

#include <functional>
#include <array>
#include <cassert>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Command::Pool);

namespace OCRA::Command::Buffer
{
enum class State {
	Initial, Recording, Executable, Pending, Invalid
};
struct Impl
{
	Impl(const Level a_Level, const Pool::Handle& a_CommandPool)
		: level(a_Level)
		, commandPool(a_CommandPool)
	{}
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
	void Begin(const CommandBufferBeginInfo& a_BeginInfo)
	{
		assert(state == State::Initial);
		state = State::Recording;
		beginInfo = a_BeginInfo;
	}
	void End()
	{
		assert(state == State::Recording);
		state = State::Executable;
	}
	void Execute()
	{
		assert(state == State::Executable);
		assert(level == Level::Primary);
		state = State::Pending;
		Execute(executionState);
		if ((beginInfo.flags & CommandBufferUsageFlagBits::OneTimeSubmit) != 0)
			Invalidate();
	}
	void ExecuteSecondary(ExecutionState& a_ExecutionState)
	{
		assert(state == State::Executable);
		assert(level == Level::Secondary);
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
	void PushCommand(const Command& a_Command)
	{
		assert(state == State::Recording);
		commands.push_back(a_Command);
	}
	const Level level;
	const Pool::WeakHandle commandPool;
	State state{ State::Initial };
	std::vector<Command> commands;
	CommandBufferBeginInfo beginInfo;
	ExecutionState executionState{};
};
std::vector<Handle> Allocate(const Device::Handle& a_Device, const AllocateInfo& a_Info) {
	std::vector<Handle> commandBuffers;
	for (auto i = 0u; i < a_Info.count; ++i)
		commandBuffers.push_back(Handle(new Impl(a_Info.level, a_Info.commandPool)));
	return commandBuffers;
}
void PushCommand(const Handle& a_CommandBuffer, const Command& a_Callback) {
	a_CommandBuffer->PushCommand(a_Callback);
}
void Execute(const std::vector<Handle>& a_CommandBuffers)
{
	for (const auto& commandBuffer : a_CommandBuffers) {
		commandBuffer->Execute();
	}
}
Pool::Handle GetPool(const Handle& a_CommandBuffer)
{
	return a_CommandBuffer->commandPool.lock();
}
}

namespace OCRA::Command
{
void BeginCommandBuffer(const Buffer::Handle& a_CommandBuffer, const CommandBufferBeginInfo& a_BeginInfo) {
	a_CommandBuffer->Begin(a_BeginInfo);
}
void EndCommandBuffer(const Buffer::Handle& a_CommandBuffer) {
	a_CommandBuffer->End();
}
void ResetCommandBuffer(const Buffer::Handle& a_CommandBuffer) {
	a_CommandBuffer->Reset();
}
void ExecuteCommands(
	const Buffer::Handle& a_CommandBuffer,
	const Buffer::Handle& a_SecondaryCommandBuffer)
{
	a_CommandBuffer->PushCommand([
		commandBuffer = a_SecondaryCommandBuffer
	](Buffer::ExecutionState& executionState) {
		commandBuffer->ExecuteSecondary(executionState);
	});
}
}