#pragma once

#include <OCRA/Structs.hpp>

#include <vector>

/**
 * One muse always make sure Command Pool outlives its Command Buffers
 */
namespace OCRA::Command::Pool {
std::vector<Command::Buffer::Handle> AllocateCommandBuffer(
    const Command::Pool::Handle& a_Pool,
    const AllocateCommandBufferInfo& a_Info);
}
