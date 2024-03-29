#pragma once

#include <OCRA/Handle.hpp>

#include <GL/CommandInterface.hpp>
#include <GL/Common/Assert.hpp>
#include <GL/ExecutionState.hpp>
#include <GL/PushConstants.hpp>

#include <functional>
#include <memory_resource>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Command::Buffer);
OCRA_DECLARE_HANDLE(OCRA::Command::Buffer);

namespace OCRA {
enum class CommandBufferState {
    Initial,
    Recording,
    Executable,
    Pending,
    Invalid,
    MaxValue
};
}

namespace OCRA::Command::Buffer {
struct CommandStorage {
    template <typename C>
    CommandStorage(C* a_Command, std::pmr::memory_resource* a_MemoryResource)
        : command(a_Command)
        , memoryResource(a_MemoryResource)
        , size(sizeof(C))
        , alignment(alignof(C))
    {
    }
    CommandStorage(CommandStorage&& a_Other) noexcept
        : command(std::move(a_Other.command))
        , memoryResource(a_Other.memoryResource)
        , size(a_Other.size)
        , alignment(a_Other.alignment)
    {
        a_Other.command = nullptr;
    }
    CommandStorage(const CommandStorage*) = delete;
    ~CommandStorage()
    {
        if (command == nullptr)
            return; // this one is empty
        std::destroy_at(command);
        memoryResource->deallocate(command, size, alignment);
    }
    inline void operator()(ExecutionState& a_ExecutionState) const
    {
        (*command)(a_ExecutionState);
    }
    CommandI* command;
    std::pmr::memory_resource* const memoryResource;
    const size_t size;
    const size_t alignment;
};

struct Impl {
    Impl(
        const Device::Handle& a_Device,
        const CommandBufferLevel& a_Level,
        const bool& a_AllowReset,
        std::pmr::memory_resource* a_MemoryResource)
        : device(a_Device)
        , level(a_Level)
        , allowReset(a_AllowReset)
        , pushConstants(a_Device)
        , memoryResource(a_MemoryResource)
    {
    }
    ~Impl() { Invalidate(); }
    void Reset();
    void Invalidate();
    void Begin(const CommandBufferBeginInfo& a_BeginInfo);
    void End();
    void ExecutePrimary();
    void ExecuteSecondary(ExecutionState& a_PrimaryExecutionState);
    void Execute(ExecutionState& a_ExecutionState);
    // moved into header and inline function for performance
    template <typename C, typename... Args>
    inline void PushCommand(Args&&... a_Args)
    {
        OCRA_ASSERT(state == CommandBufferState::Recording);
        auto commandStorage = memoryResource->allocate(sizeof(C), alignof(C));
        auto command        = new (commandStorage) C(std::forward<Args>(a_Args)...);
        commands.push_back({ command, memoryResource });
    }
    const Device::WeakHandle device;
    const CommandBufferLevel level;
    const bool allowReset;
    CommandBufferState state { CommandBufferState::Initial };
    CommandBufferUsageFlags usageFlags;
    OCRA::PushConstants pushConstants;
    ExecutionState executionState;
    std::pmr::memory_resource* memoryResource; // the command pool's memory resource
    std::pmr::vector<WeakHandle> parentBuffers { memoryResource };
    std::pmr::vector<CommandStorage> commands { memoryResource };
};
static inline void Execute(const std::vector<Handle>& a_CommandBuffers)
{
    for (const auto& commandBuffer : a_CommandBuffers) {
        commandBuffer->ExecutePrimary();
    }
}
}
