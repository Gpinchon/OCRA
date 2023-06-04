/*
 * @Author: gpinchon
 * @Date:   2021-09-26 00:00:00
 * @Last Modified by:   gpinchon
 * @Last Modified time: 2021-09-26 14:26:36
 */
#pragma once

#include <OCRA/Handles.hpp>
#include <OCRA/Structs.hpp>

/**
 * Command buffers can hold refs to pooled objects and should always be destroyed first when exiting application
 */
namespace OCRA::Command::Buffer {
// Begin Command Buffer recording, switching it to Recording state
void Begin(const Handle& a_CommandBuffer,
    const CommandBufferBeginInfo& a_BeginInfo);
// End Command Buffer recording, switching it to Executable state
void End(const Handle& a_CommandBuffer);
// Reset Command Buffer to Initial state
void Reset(const Handle& a_CommandBuffer);
}
