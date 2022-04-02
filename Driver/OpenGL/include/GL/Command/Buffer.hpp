#pragma once

#include <Handle.hpp>

#include <functional>
#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Command::Buffer);
OCRA_DECLARE_HANDLE(OCRA::Queue);

namespace OCRA::Command::Buffer
{
struct ExecutionState;
typedef std::function<void(ExecutionState&)> Command;
void PushCommand(const Handle& a_CommandBuffer, const Command& a_Callback);
void Execute(const std::vector<Handle>& a_CommandBuffers);
}