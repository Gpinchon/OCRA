#pragma once

#include <OCRA/ShaderCompiler/Handles.hpp>
#include <OCRA/ShaderCompiler/Structs.hpp>

#include <vector>

namespace OCRA::ShaderCompiler
{
Handle Create();
std::vector<uint32_t> Compile(const Handle& a_Compiler, const ShaderInfo& a_Info);
}