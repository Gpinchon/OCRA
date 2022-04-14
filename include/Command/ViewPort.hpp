#pragma once

#include <Common/ViewPort.hpp>
#include <Handle.hpp>

#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Command::Buffer);

namespace OCRA::Command
{
void SetViewPort(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const uint32_t&                 a_FirstViewPort,
    const std::vector<ViewPort>&    a_ViewPorts);
}