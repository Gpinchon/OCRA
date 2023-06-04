#pragma once

#include <OCRA/Handles.hpp>

namespace OCRA::Buffer {
/**
 * @brief Binds the specified memory to the specified Buffer.
 * On OpenGL, this will update internal structure to use a certain range of the GPU buffer.
 * This comes with limitations, especially regarding indices and instanciated draw calls.
 */
void BindMemory(
    const Handle& a_Buffer,
    const Memory::Handle& a_Memory,
    const size_t& a_MemoryOffset);

/**
 * @return the required size for Memory
 */
size_t GetSizeRequirement(
    const Handle& a_Buffer);

/**
 * @return the memory bound to the specified buffer
 */
Memory::Handle GetMemory(const Handle& a_Buffer);
}
