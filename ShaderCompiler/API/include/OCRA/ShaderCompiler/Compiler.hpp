#pragma once

#include <OCRA/ShaderCompiler/Handles.hpp>
#include <OCRA/ShaderCompiler/Structs.hpp>

#include <vector>

namespace OCRA::ShaderCompiler {
Handle Create();
CompiledShader Compile(const Handle& a_Compiler, const ShaderInfo& a_Info);
}
