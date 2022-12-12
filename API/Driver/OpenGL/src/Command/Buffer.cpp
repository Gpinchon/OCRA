/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/

#include <OCRA/Command/Buffer.hpp>
#include <OCRA/Queue/Queue.hpp>

#include <GL/Command/Buffer.hpp>
#include <GL/Command/ExecutionState.hpp>

#include <functional>
#include <array>

namespace OCRA::Command::Buffer
{
void Begin(const Handle& a_CommandBuffer, const BeginInfo& a_BeginInfo) {
    a_CommandBuffer->Begin(a_BeginInfo);
}
void End(const Handle& a_CommandBuffer) {
    a_CommandBuffer->End();
}
void Reset(const Handle& a_CommandBuffer) {
    a_CommandBuffer->Reset();
}
}

namespace OCRA::Command
{
void ExecuteCommandBuffer(
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
void PushConstants(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Pipeline::Layout::Handle& a_PipelineLayout,
    const uint8_t a_Offset,
    const std::vector<std::byte>& a_Data)
{
    a_CommandBuffer->PushCommand([
        pipelineLayout = a_PipelineLayout,
        offset = size_t(a_Offset),
        data = a_Data
    ](Buffer::ExecutionState& a_ExecutionState) {
        a_ExecutionState.pushConstants.Update(offset, data);
    });
}
}

void OCRA::Command::Buffer::Impl::Reset()
{
    assert(
        state == State::Initial    ||
        state == State::Recording  ||
        state == State::Executable ||
        state == State::Invalid
    );
    state = State::Initial;
    commands.clear();
    secondaryBuffers.clear();
    executionState.Reset();
}

void OCRA::Command::Buffer::Impl::Invalidate()
{
    assert(
        state == State::Recording  ||
        state == State::Executable ||
        state == State::Pending
    );
    state = State::Invalid;
    commands.clear();
    secondaryBuffers.clear();
    executionState.Reset();
}

void OCRA::Command::Buffer::Impl::Begin(const Buffer::BeginInfo& a_BeginInfo)
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

void OCRA::Command::Buffer::Impl::ExecutePrimary()
{
    assert(level == Level::Primary);
    Execute(executionState);
}

void OCRA::Command::Buffer::Impl::ExecuteSecondary(ExecutionState& a_ExecutionState)
{
    assert(level == Level::Secondary);
    Execute(a_ExecutionState);
}

void OCRA::Command::Buffer::Impl::Execute(ExecutionState& a_ExecutionState)
{
    assert(state == State::Executable);
    state = State::Pending;
    for (const auto& command : commands)
        command(a_ExecutionState);
    if ((beginInfo.flags & UsageFlagBits::OneTimeSubmit) != 0)
        Invalidate();
    else state = State::Executable;
}
