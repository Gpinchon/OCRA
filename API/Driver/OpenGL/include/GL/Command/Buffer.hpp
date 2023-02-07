#pragma once

#include <OCRA/Handle.hpp>
#include <OCRA/Command/Buffer.hpp>

#include <GL/Command/ExecutionState.hpp>
#include <GL/PushConstants.hpp>

#include <functional>
#include <memory_resource>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Command::Buffer);
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
    Impl(const Device::Handle& a_Device, const Level a_Level, std::pmr::memory_resource* a_MemoryResource)
        : level(a_Level)
        , pushConstants(a_Device)
        , commands(a_MemoryResource)
        , parentBuffers(a_MemoryResource)
    {}
    ~Impl() { Invalidate(); }
    void Reset();
    void Invalidate();
    void Begin(const Buffer::BeginInfo& a_BeginInfo);
    void End();
    void ExecutePrimary();
    void ExecuteSecondary(ExecutionState& a_PrimaryExecutionState);
    void Execute(ExecutionState& a_ExecutionState);
    //moved into header and inline function for performance
    inline void PushCommand(const Command& a_Command)
    {
        assert(state == State::Recording);
        commands.push_back(a_Command);
    }
    const Level level;
    State       state{ State::Initial };
    UsageFlags  usageFlags;
    OCRA::PushConstants     pushConstants;
    ExecutionState          executionState;
    std::pmr::vector<Command>    commands;
    std::pmr::vector<WeakHandle> parentBuffers;
};
static inline void Execute(const std::vector<Handle>& a_CommandBuffers) {
    for (const auto& commandBuffer : a_CommandBuffers) {
        commandBuffer->ExecutePrimary();
    }
}
}
