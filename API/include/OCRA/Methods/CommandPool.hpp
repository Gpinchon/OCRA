#pragma once

#include <OCRA/Structs.hpp>

#include <vector>

/**
* One muse always make sure Command Pool outlives its Command Buffers
*/
namespace OCRA::Command::Pool
{
std::vector<Buffer::Handle> AllocateCommandBuffer(const AllocateCommandBufferInfo& a_Info);
}