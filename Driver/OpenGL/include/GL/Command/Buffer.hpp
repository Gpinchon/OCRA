#pragma once

#include <functional>

#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Command::Buffer);

namespace OCRA::Command::Buffer
{
struct ExecutionState;
typedef std::function<void(ExecutionState&)> CallBack;
void PushCommand(const Handle& a_CommandBuffer, const CallBack& a_Callback);
void Submit(const std::vector<Command::Buffer::Handle>& a_CommandBuffers);
}