#pragma once

#include <Handle.hpp>
#include <Command/Buffer.hpp>

#include <GL/Command/ExecutionState.hpp>

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
	Impl(const Level a_Level) : level(a_Level) {}
	~Impl() { Invalidate(); }
	void Reset();
	void Invalidate();
	void Begin(const CommandBufferBeginInfo& a_BeginInfo);
	void End();
	void Execute();
	void ExecuteSecondary(ExecutionState& a_ExecutionState);
	void Execute(ExecutionState& a_ExecutionState);
	void PushCommand(const Command& a_Command);
	const Level level;
	State state{ State::Initial };
	std::vector<Command> commands;
	std::vector<Handle> secondaryBuffers;
	CommandBufferBeginInfo beginInfo;
	ExecutionState executionState{};
};
static inline void Execute(const std::vector<Handle>& a_CommandBuffers) {
	for (const auto& commandBuffer : a_CommandBuffers) {
		commandBuffer->Execute();
	}
}
}
