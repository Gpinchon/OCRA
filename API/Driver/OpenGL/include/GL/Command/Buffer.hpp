#pragma once

#include <OCRA/Handle.hpp>
#include <OCRA/Command/Buffer.hpp>

#include <GL/Command/ExecutionState.hpp>
#include <GL/PushConstants.hpp>

#include <functional>
#include <vector>
#include <cassert>

OCRA_DECLARE_HANDLE(OCRA::Command::Buffer);

namespace OCRA::Command::Buffer
{
typedef std::function<void(ExecutionState&)> Command;
enum class State {
    Initial, Recording, Executable, Pending, Invalid
};
enum class Level {
    Unknown = -1, Primary, Secondary
};
struct Impl
{
    Impl(const Device::Handle& a_Device)
        : level(Level::Unknown)
        , pushConstants(a_Device)
    {}
    Impl(const Device::Handle& a_Device, const Level a_Level)
        : level(a_Level)
        , pushConstants(a_Device)
    {}
    ~Impl() { Invalidate(); }
    void Reset();
    void Invalidate();
    void Begin(const Buffer::BeginInfo& a_BeginInfo);
    void End();
    void ExecutePrimary();
    void ExecuteSecondary(ExecutionState& a_ExecutionState);
    void Execute(ExecutionState& a_ExecutionState);
    //moved into header and inline function for performance
    inline void PushCommand(const Command& a_Command)
    {
        assert(state == State::Recording);
        commands.push_back(a_Command);
    }
    const Level level;
    State state{ State::Initial };
    std::vector<Command> commands;
    std::vector<Handle> secondaryBuffers;
    Buffer::BeginInfo beginInfo;
    ExecutionState executionState{};
    OCRA::PushConstants pushConstants;
};
static inline void Execute(const std::vector<Handle>& a_CommandBuffers) {
    for (const auto& commandBuffer : a_CommandBuffers) {
        commandBuffer->ExecutePrimary();
    }
}
}
