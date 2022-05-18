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
    /**
    * @arg a_Name : the shader's entry point
    * @arg a_Source : the shader's human readable source code
    * @arg a_Type : the shader's type (Vertex, Geometry, Fragment...)
    */
    Shader(const std::string& a_Name, const std::string& a_Source, const Type& a_Type)
        : name(a_Name), source(a_Source), type(a_Type)
    {}
    //compile to SPIRV binary words
    std::vector<uint32_t> Compile(bool a_Optimize);
    const std::string name;
    const std::string source;
    const Type type;
};
//helper structs
struct VertexShader : Shader {
    VertexShader(const std::string& a_Name, const std::string& a_Source) : Shader(a_Name, a_Source, Type::Vertex) {}
};
struct GeometryShader : Shader {
    GeometryShader(const std::string& a_Name, const std::string& a_Source) : Shader(a_Name, a_Source, Type::Geometry) {}
};
struct FragmentShader : Shader {
    FragmentShader(const std::string& a_Name, const std::string& a_Source) : Shader(a_Name, a_Source, Type::Fragment) {}
};
struct ComputeShader : Shader {
    ComputeShader(const std::string& a_Name, const std::string& a_Source) : Shader(a_Name, a_Source, Type::Compute) {}
};
struct TessControlShader : Shader {
    TessControlShader(const std::string& a_Name, const std::string& a_Source) : Shader(a_Name, a_Source, Type::TessControl) {}
};
struct TessEvalShader : Shader {
    TessEvalShader(const std::string& a_Name, const std::string& a_Source) : Shader(a_Name, a_Source, Type::TessEval) {}
};
}