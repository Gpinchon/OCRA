#pragma once

namespace OCRA::ShaderCompiler {
enum class TargetAPI {
    Unknown = -1,
    OpenGL,
    Vulkan,
    DirectX,
    MaxValue
};
enum class ShaderType {
    Unknown = -1,
    Vertex,
    Geometry,
    Fragment,
    Compute,
    TessControl,
    TessEval,
    MaxValue
};
enum class ShaderResourceBaseType {
    Unknown = -1,
    Void,
    Boolean,
    SByte,
    UByte,
    Short,
    UShort,
    Int,
    UInt,
    Int64,
    UInt64,
    AtomicCounter,
    Half,
    Float,
    Double,
    Struct,
    Image,
    SampledImage,
    Sampler,
    AccelerationStructure,
    RayQuery
};
}
