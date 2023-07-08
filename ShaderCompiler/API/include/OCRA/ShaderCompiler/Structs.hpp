#pragma once

#include <OCRA/ShaderCompiler/Enums.hpp>

#include <optional>
#include <string>
#include <vector>

namespace OCRA::ShaderCompiler {
struct ShaderInfo {
    TargetAPI targetAPI;
    ShaderType type { ShaderType::Unknown }; // the shader's type (Vertex, Geometry, Fragment...)
    std::string entryPoint {}; // the shader's entry point
    std::string source {}; // the shader's human readable source code
};

template <typename It>
struct ShaderResourcesRange {
    auto& at(size_t a_Index)
    {
        return *(begin() + a_Index);
    }
    auto& begin() { return begIt; }
    auto& end() { return endIt; }
    It begIt;
    It endIt;
};

template <typename ResourceType>
struct ShaderStructure {
    uint32_t byteSize; // the total byte size of the struct
    std::vector<std::pair<std::string, ResourceType>> members; // a vector of pair of the name and the type
};

struct ShaderResourceType {
    ShaderResourceBaseType baseType;
    uint32_t rows    = 0; // 1 for float, 2 for vec2, 3 for vec3...
    uint32_t columns = 0; // 1 for float, vec; 4 for mat4
    uint32_t width   = 0; // bitSize of the components 64 for double, 32 for float, 16 for short
    std::optional<ShaderStructure<ShaderResourceType>> structure; // only applicable to structs
};

struct ShaderResource {
    uint32_t binding  = -1;
    uint32_t location = -1;
    std::string name;
    ShaderResourceType type;
};

struct ShaderResources {
    using Range = ShaderResourcesRange<std::vector<ShaderResource>::iterator>;
    Range GetPushConstants()
    {
        return {
            resources.begin() + pushConstants.front(),
            resources.begin() + pushConstants.back()
        };
    }
    Range GetSampledImages()
    {
        return {
            resources.begin() + sampledImages.front(),
            resources.begin() + sampledImages.back()
        };
    }
    Range GetStageInputs()
    {
        return {
            resources.begin() + stageInputs.front(),
            resources.begin() + stageInputs.back()
        };
    }
    Range GetStageOutputs()
    {
        return {
            resources.begin() + stageOutputs.front(),
            resources.begin() + stageOutputs.back()
        };
    }
    Range GetUniformBuffers()
    {
        return {
            resources.begin() + uniformBuffers.front(),
            resources.begin() + uniformBuffers.back()
        };
    }
    std::vector<uint32_t> pushConstants;
    std::vector<uint32_t> sampledImages;
    std::vector<uint32_t> stageInputs;
    std::vector<uint32_t> stageOutputs;
    std::vector<uint32_t> uniformBuffers;
    std::vector<ShaderResource> resources;
};

struct CompiledShader : ShaderInfo {
    CompiledShader(const ShaderInfo& a_Info)
        : ShaderInfo(a_Info)
    {
    }
    std::vector<uint32_t> SPIRVBinary; // SPIRV code blob to use with specified API
    ShaderResources resources; // parsed shader resources, uniform, stage IO...
};
}
