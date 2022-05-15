#pragma once

#include <string>

namespace OCRA::ShaderCompiler
{
class Shader
{
    Shader(const std::string& a_Code);
    const std::string code;
};
}