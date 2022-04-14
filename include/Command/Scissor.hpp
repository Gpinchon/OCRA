#pragma once

#include <Common/Rect2D.hpp>
#include <Handle.hpp>

#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Command::Buffer);

namespace OCRA::Command
{
void SetScissor(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const uint32_t&                 a_FirstScissor,
    const std::vector<iRect2D>&     a_Scissors);
}