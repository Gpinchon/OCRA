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
struct ShaderStructureMember {
    ShaderStructureMember(const std::string& a_Name, const ResourceType& a_Type)
        : name(a_Name)
        , type(a_Type)
    {}
    std::string  name;
    ResourceType type;
};

template <typename ResourceType>
struct ShaderStructure {
    using MemberType = ShaderStructureMember<ResourceType>;
    MemberType& operator[](const size_t a_MemberIndex) { return members[a_MemberIndex]; }
    MemberType& at(const size_t a_MemberIndex) { return members.at(a_MemberIndex); }
    uint32_t byteSize; // the total byte size of the struct
    std::vector<MemberType> members; // a vector of pair of the name and the type
};

struct ShaderResourceType {
    ShaderResourceBaseType baseType;
    uint32_t rows      = 0; // 1 for float, 2 for vec2, 3 for vec3...
    uint32_t columns   = 0; // 1 for float, vec; 4 for mat4
    uint32_t byteWidth = 0; // byte size of the components 8 for double, 4 for float, 2 for short
    std::vector<uint32_t> arraySize; //arraySize.size is array dimension, 0 for non array types, array.at(0) will be array size, 0 for SSBO
    std::optional<ShaderStructure<ShaderResourceType>> structure; // only applicable to structs
};

struct ShaderResource {
    uint32_t binding  = -1;
    uint32_t location = -1;
    std::string name;
    ShaderResourceType type;
};

struct CompiledShader : ShaderInfo {
    using ResourceRange = ShaderResourcesRange<std::vector<ShaderResource>::iterator>;
    CompiledShader(const ShaderInfo& a_Info)
        : ShaderInfo(a_Info)
    {
    }
    ResourceRange GetPushConstants()
    {
        return {
            resources.begin() + pushConstants.front(),
            resources.begin() + pushConstants.back() + 1
        };
    }
    ResourceRange GetSampledImages()
    {
        return {
            resources.begin() + sampledImages.front(),
            resources.begin() + sampledImages.back() + 1
        };
    }
    ResourceRange GetSeparateImages()
    {
        return {
            resources.begin() + separateImages.front(),
            resources.begin() + separateImages.back() + 1
        };
    }
    ResourceRange GetStageInputs()
    {
        return {
            resources.begin() + stageInputs.front(),
            resources.begin() + stageInputs.back() + 1
        };
    }
    ResourceRange GetStageOutputs()
    {
        return {
            resources.begin() + stageOutputs.front(),
            resources.begin() + stageOutputs.back() + 1
        };
    }
    ResourceRange GetStorageImages()
    {
        return {
            resources.begin() + storageImages.front(),
            resources.begin() + storageImages.back() + 1
        };
    }
    ResourceRange GetUniformBuffers()
    {
        return {
            resources.begin() + uniformBuffers.front(),
            resources.begin() + uniformBuffers.back() + 1
        };
    }
    std::vector<uint32_t> SPIRVBinary; // SPIRV code blob to use with specified API
    std::vector<uint32_t> pushConstants;
    std::vector<uint32_t> sampledImages;
    std::vector<uint32_t> separateImages;
    std::vector<uint32_t> stageInputs;
    std::vector<uint32_t> stageOutputs;
    std::vector<uint32_t> storageBuffers;
    std::vector<uint32_t> storageImages;
    std::vector<uint32_t> uniformBuffers;
    std::vector<ShaderResource> resources;
};
}
