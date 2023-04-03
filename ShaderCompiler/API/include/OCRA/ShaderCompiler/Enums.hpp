#pragma once

namespace OCRA::ShaderCompiler
{
enum class TargetAPI {
    Unknown = -1,
    OpenGL, Vulkan, DirectX,
    MaxValue
};
enum class ShaderType {
    Unknown = -1,
    Vertex, Geometry, Fragment,
    Compute,
    TessControl, TessEval,
    MaxValue
};
}