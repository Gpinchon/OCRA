#pragma once

#include <Handle.hpp>

OCRA_DECLARE_HANDLE(OCRA::Memory);

namespace OCRA::Memory
{
uint32_t GetGLHandle(const Handle& a_Memory);
}