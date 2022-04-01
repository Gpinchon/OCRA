#pragma once

#include <Handle.hpp>

OCRA_DECLARE_HANDLE(OCRA::Buffer);
OCRA_DECLARE_HANDLE(OCRA::Memory);

namespace OCRA::Buffer
{
struct MemoryBinding {
	Memory::Handle	memory;
	size_t			memoryOffset{ 0 };
};
const MemoryBinding& GetMemoryBinding(const Handle& a_Buffer);
uint32_t GetGLHandle(const Handle& a_Buffer);
}