#pragma once

#include <string>
#include <vector>

namespace OCRA::ShaderCompiler
{
struct Shader
{
    enum class Type {
        Vertex, Geometry, Fragment,
        Compute,
        TessControl, TessEval,
        MaxValue
    };
    Shader(const std::string& a_Name, const std::string& a_Code, const Type& a_Type)
        : name(a_Name), code(a_Code), type(a_Type)
    {}
    //compile to SPIRV binary words
    std::vector<uint32_t> Compile(bool a_Optimize);
    const std::string name;
    const std::string code;
    const Type type;
};
//helper structs
struct VertexShader : Shader {
    VertexShader(const std::string& a_Name, const std::string& a_Code) : Shader(a_Name, a_Code, Type::VertexShader) {}
};
struct GeometryShader : Shader {
    GeometryShader(const std::string& a_Name, const std::string& a_Code) : Shader(a_Name, a_Code, Type::GeometryShader) {}
};
struct FragmentShader : Shader {
    FragmentShader(const std::string& a_Name, const std::string& a_Code) : Shader(a_Name, a_Code, Type::FragmentShader) {}
};
struct ComputeShader : Shader {
    ComputeShader(const std::string& a_Name, const std::string& a_Code) : Shader(a_Name, a_Code, Type::ComputeShader) {}
};
struct TessControlShader : Shader {
    TessControlShader(const std::string& a_Name, const std::string& a_Code) : Shader(a_Name, a_Code, Type::TessControlShader) {}
};
struct TessEvalShader : Shader {
    TessEvalShader(const std::string& a_Name, const std::string& a_Code) : Shader(a_Name, a_Code, Type::TessEvalShader) {}
};
}