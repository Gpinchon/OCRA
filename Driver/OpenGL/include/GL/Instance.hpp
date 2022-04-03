#pragma once

#include <Handle.hpp>

#include <functional>

OCRA_DECLARE_HANDLE(OCRA::Instance);

#ifdef _WIN32
#include <windows.h>
#endif

namespace OCRA::Instance
{
void PushCommand(const Handle& a_Instance, const std::function<void()>& a_Command, const bool a_Synchronous);
void MakeCurrent(const Handle& a_Instance, const HDC& a_HDC);
}