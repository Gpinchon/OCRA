#pragma once

#include <functional>

OCRA_DECLARE_HANDLE(OCRA::Command::Buffer);

namespace OCRA::Command::Buffer
{
struct ExecutionState;
typedef std::function<void(ExecutionState&)> CallBack;
void PushCommand(const Handle& a_CommandBuffer, const CallBack& a_Callback);
}