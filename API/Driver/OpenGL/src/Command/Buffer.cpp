/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/

#include <OCRA/Command/Buffer.hpp>
#include <OCRA/Queue.hpp>

#include <GL/Common/Assert.hpp>
#include <GL/Command/Buffer.hpp>
#include <GL/Command/ExecutionState.hpp>

#include <functional>
#include <array>

void OCRA::Command::Buffer::Impl::Reset()
{
    OCRA_ASSERT(
        state == State::Initial    ||
        state == State::Recording  ||
        state == State::Executable ||
        state == State::Invalid
    );
    state = State::Initial;
    commands.clear();
    executionState.Reset();
    for (auto& parentBuffer : parentBuffers) {
        auto parentBufferPtr = parentBuffer.lock();
        auto parentState = parentBufferPtr->state;
        if (parentState == State::Recording ||
            parentState == State::Executable)
            parentBufferPtr->Invalidate();
    }
}

void OCRA::Command::Buffer::Impl::Invalidate()
{
    OCRA_ASSERT(
        state == State::Recording  ||
        state == State::Executable ||
        state == State::Pending
    );
    state = State::Invalid;
    commands.clear();
    executionState.Reset();
}

void OCRA::Command::Buffer::Impl::Begin(const Buffer::BeginInfo& a_BeginInfo)
{
    OCRA_ASSERT(state == State::Initial);
    state = State::Recording;
    usageFlags = a_BeginInfo.flags;
    if (level == Level::Secondary) {
        auto& inheritanceInfo = a_BeginInfo.inheritanceInfo.value(); //should throw if empty
        if ((usageFlags & UsageFlagBits::RenderPassContinue) != 0)
        {
            executionState.subpassIndex = inheritanceInfo.subpass;
            executionState.renderPass.renderPass = inheritanceInfo.renderPass;
            executionState.renderPass.framebuffer = inheritanceInfo.framebuffer;
        }
    }
}

void OCRA::Command::Buffer::Impl::End()
{
    OCRA_ASSERT(state == State::Recording);
    state = State::Executable;
}

void OCRA::Command::Buffer::Impl::ExecutePrimary()
{
    OCRA_ASSERT(level == Level::Primary);
    Execute(executionState);
}

void OCRA::Command::Buffer::Impl::ExecuteSecondary(ExecutionState& a_PrimaryExecutionState)
{
    OCRA_ASSERT(level == Level::Secondary);
    if ((usageFlags & UsageFlagBits::RenderPassContinue) != 0) {
        executionState.renderPass = a_PrimaryExecutionState.renderPass;
        executionState.renderPass.indexBufferBinding = {};
        executionState.renderPass.vertexInputBindings.clear();
    }
    Execute(executionState);
}

void OCRA::Command::Buffer::Impl::Execute(ExecutionState& a_ExecutionState)
{
    OCRA_ASSERT(state == State::Executable);
    state = State::Pending;
    for (const auto& command : commands)
        command(a_ExecutionState);
    if ((usageFlags & UsageFlagBits::OneTimeSubmit) != 0)
        Invalidate();
    else {
        executionState.Reset();
        state = State::Executable;
    }
}

void OCRA::Command::Buffer::Begin(const Handle& a_CommandBuffer, const BeginInfo& a_BeginInfo) {
    a_CommandBuffer->Begin(a_BeginInfo);
}

void OCRA::Command::Buffer::End(const Handle& a_CommandBuffer) {
    a_CommandBuffer->End();
}

void OCRA::Command::Buffer::Reset(const Handle& a_CommandBuffer) {
    a_CommandBuffer->Reset();
}