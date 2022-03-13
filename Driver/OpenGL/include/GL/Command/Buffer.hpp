#pragma once

#include <Command/Buffer.hpp>

#include <functional>

namespace OCRA::Command::Buffer
{
struct ExecutionState;
typedef std::function<void(ExecutionState&)> CallBack;
void PushCommand(const Handle& a_CommandBuffer, const CallBack& a_Callback);
}