/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/

#include <OCRA/OCRA.hpp>

#include <GL/Common/Assert.hpp>
#include <GL/CommandBuffer.hpp>
#include <GL/ExecutionState.hpp>

#include <functional>
#include <array>

void OCRA::Command::Buffer::Impl::Reset()
{
    OCRA_ASSERT(
        state == CommandBufferState::Initial    ||
        state == CommandBufferState::Recording  ||
        state == CommandBufferState::Executable ||
        state == CommandBufferState::Invalid
    );
    state = CommandBufferState::Initial;
    commands.clear();
    executionState.Reset();
    for (auto& parentBuffer : parentBuffers) {
        auto parentBufferPtr = parentBuffer.lock();
        auto parentState = parentBufferPtr->state;
        if (parentState == CommandBufferState::Recording ||
            parentState == CommandBufferState::Executable)
            parentBufferPtr->Invalidate();
    }
}

void OCRA::Command::Buffer::Impl::Invalidate()
{
    OCRA_ASSERT(
        state == CommandBufferState::Recording  ||
        state == CommandBufferState::Executable ||
        state == CommandBufferState::Pending
    );
    state = CommandBufferState::Invalid;
    commands.clear();
    executionState.Reset();
}

void OCRA::Command::Buffer::Impl::Begin(const CommandBufferBeginInfo& a_BeginInfo)
{
    OCRA_ASSERT(state == CommandBufferState::Initial);
    state = CommandBufferState::Recording;
    usageFlags = a_BeginInfo.flags;
    if (level == CommandBufferLevel::Secondary) {
        //auto& inheritanceInfo = a_BeginInfo.inheritanceInfo.value(); //should throw if empty
        if ((usageFlags & CommandBufferUsageFlagBits::RenderPassContinue) != 0)
        {
            //executionState.subpassIndex = inheritanceInfo.subpass;
            //executionState.renderPass.renderPass = inheritanceInfo.renderPass;
            //executionState.renderPass.framebuffer = inheritanceInfo.framebuffer;
        }
    }
}

void OCRA::Command::Buffer::Impl::End()
{
    OCRA_ASSERT(state == CommandBufferState::Recording);
    state = CommandBufferState::Executable;
}

void OCRA::Command::Buffer::Impl::ExecutePrimary()
{
    OCRA_ASSERT(level == CommandBufferLevel::Primary);
    Execute(executionState);
}

void OCRA::Command::Buffer::Impl::ExecuteSecondary(ExecutionState& a_PrimaryExecutionState)
{
    OCRA_ASSERT(level == CommandBufferLevel::Secondary);
    if ((usageFlags & CommandBufferUsageFlagBits::RenderPassContinue) != 0) {
        executionState.renderPass = a_PrimaryExecutionState.renderPass;
        executionState.renderPass.indexBufferBinding = {};
        executionState.renderPass.vertexInputBindings.clear();
    }
    Execute(executionState);
}

void OCRA::Command::Buffer::Impl::Execute(ExecutionState& a_ExecutionState)
{
    OCRA_ASSERT(state == CommandBufferState::Executable);
    state = CommandBufferState::Pending;
    for (const auto& command : commands)
        command(a_ExecutionState);
    if ((usageFlags & CommandBufferUsageFlagBits::OneTimeSubmit) != 0)
        Invalidate();
    else {
        executionState.Reset();
        state = CommandBufferState::Executable;
    }
}

void OCRA::Command::Buffer::Begin(const Handle& a_CommandBuffer, const CommandBufferBeginInfo& a_BeginInfo) {
    a_CommandBuffer->Begin(a_BeginInfo);
}

void OCRA::Command::Buffer::End(const Handle& a_CommandBuffer) {
    a_CommandBuffer->End();
}

void OCRA::Command::Buffer::Reset(const Handle& a_CommandBuffer) {
    a_CommandBuffer->Reset();
}