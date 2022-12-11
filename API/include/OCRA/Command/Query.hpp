#pragma once

#include <OCRA/Handle.hpp>

OCRA_DECLARE_HANDLE(OCRA::Command::Buffer);
OCRA_DECLARE_HANDLE(OCRA::QueryPool);

namespace OCRA::Command
{
void BeginQuery(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const QueryPool::Handle&        a_QueryPool,
    const uint32_t&                 a_QueryIndex);
}