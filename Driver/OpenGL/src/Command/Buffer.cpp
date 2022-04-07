/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/

#include <Command/Buffer.hpp>
#include <Queue/Queue.hpp>

#include <GL/WeakHandle.hpp>
#include <GL/Command/Buffer.hpp>
#include <GL/Command/ExecutionState.hpp>

#include <functional>
#include <array>

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
	a_CommandBuffer->secondaryBuffers.push_back(a_SecondaryCommandBuffer);
	a_CommandBuffer->PushCommand([
		commandBuffer = a_SecondaryCommandBuffer.get()
	](Buffer::ExecutionState& executionState) {
		commandBuffer->ExecuteSecondary(executionState);
	});
}
}

void OCRA::Command::Buffer::Impl::Reset()
{
    assert(
        state == State::Recording ||
        state == State::Executable ||
        state == State::Invalid
    );
    state = State::Initial;
    commands.clear();
    secondaryBuffers.clear();
    executionState = {};
}

void OCRA::Command::Buffer::Impl::Invalidate()
{
    assert(
        state == State::Recording ||
        state == State::Executable ||
        state == State::Pending
    );
    state = State::Invalid;
    commands.clear();
    secondaryBuffers.clear();
    executionState = {};
}

void OCRA::Command::Buffer::Impl::Begin(const CommandBufferBeginInfo& a_BeginInfo)
{
    assert(state == State::Initial);
    state = State::Recording;
    beginInfo = a_BeginInfo;
}

void OCRA::Command::Buffer::Impl::End()
{
    assert(state == State::Recording);
    state = State::Executable;
}

void OCRA::Command::Buffer::Impl::Execute()
{
    assert(state == State::Executable);
    assert(level == Level::Primary);
    state = State::Pending;
    Execute(executionState);
}

void OCRA::Command::Buffer::Impl::ExecuteSecondary(ExecutionState& a_ExecutionState)
{
    assert(state == State::Executable);
    assert(level == Level::Secondary);
    state = State::Pending;
    Execute(a_ExecutionState);
    if ((beginInfo.flags & CommandBufferUsageFlagBits::OneTimeSubmit) != 0)
        Invalidate();
}

void OCRA::Command::Buffer::Impl::Execute(ExecutionState& a_ExecutionState)
{
    assert(state == State::Pending);
    for (const auto& command : commands)
        command(a_ExecutionState);
    state = State::Executable;
}

void OCRA::Command::Buffer::Impl::PushCommand(const Command& a_Command)
{
    assert(state == State::Recording);
    commands.push_back(a_Command);
}
