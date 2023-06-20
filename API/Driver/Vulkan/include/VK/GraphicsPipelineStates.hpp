#pragma once

#include <OCRA/Core.hpp>

#include <VK/Enums.hpp>
#include <VK/ShaderModule.hpp>
#include <VK/Structs.hpp>
#include <VK/ToVk.hpp>

#include <vulkan/vulkan_raii.hpp>

namespace OCRA {
static inline auto GetStageBits(const ShaderStageFlags& a_Flags)
{
    if (a_Flags == ShaderStageFlagBits::Compute)
        return vk::ShaderStageFlagBits::eCompute;
    if (a_Flags == ShaderStageFlagBits::Fragment)
        return vk::ShaderStageFlagBits::eFragment;
    if (a_Flags == ShaderStageFlagBits::Geometry)
        return vk::ShaderStageFlagBits::eGeometry;
    if (a_Flags == ShaderStageFlagBits::TessControl)
        return vk::ShaderStageFlagBits::eTessellationControl;
    if (a_Flags == ShaderStageFlagBits::TessEval)
        return vk::ShaderStageFlagBits::eTessellationEvaluation;
    if (a_Flags == ShaderStageFlagBits::Vertex)
        return vk::ShaderStageFlagBits::eVertex;
    return vk::ShaderStageFlagBits {};
}

struct IntermediateShaderStage {
    IntermediateShaderStage() = default;
    IntermediateShaderStage(const PipelineShaderStage& a_Stage)
        : entryPoint(a_Stage.entryPoint)
        , mapEntries(a_Stage.specializationInfo.mapEntries.size())
        , specializationData(a_Stage.specializationInfo.data)
        , specializationInfo(mapEntries.size(), mapEntries.data(), specializationData.size(), specializationData.data())
        , info({}, GetStageBits(a_Stage.stage), **a_Stage.module, entryPoint.c_str(), &specializationInfo)
    {
        std::transform(
            a_Stage.specializationInfo.mapEntries.begin(), a_Stage.specializationInfo.mapEntries.end(),
            mapEntries.begin(), ToVk {});
    }
    IntermediateShaderStage& operator=(const IntermediateShaderStage& a_Other)
    {
        entryPoint         = (a_Other.entryPoint);
        mapEntries         = (a_Other.mapEntries);
        specializationData = (a_Other.specializationData);
        specializationInfo = { uint32_t(mapEntries.size()), mapEntries.data(), specializationData.size(), specializationData.data() };
        info               = { {}, a_Other.info.stage, a_Other.info.module, entryPoint.c_str(), &specializationInfo };
        return *this;
    };
    operator auto &() const
    {
        return info;
    }
    std::string entryPoint;
    std::vector<vk::SpecializationMapEntry> mapEntries;
    std::vector<uint8_t> specializationData;
    vk::SpecializationInfo specializationInfo;
    vk::PipelineShaderStageCreateInfo info;
};

struct IntermediateShaderState {
    IntermediateShaderState(const PipelineShaderPipelineState& a_State)
        : stages(a_State.stages.size())
        , vkStages(stages.size())
    {
        std::transform(a_State.stages.begin(), a_State.stages.end(), stages.begin(),
            [](auto& stage) { return IntermediateShaderStage(stage); });
        std::transform(stages.begin(), stages.end(), vkStages.begin(),
            [](auto& stage) { return vk::PipelineShaderStageCreateInfo(stage); });
    }
    inline auto size() const
    {
        return vkStages.size();
    }
    inline auto data() const
    {
        return vkStages.data();
    }
    std::vector<IntermediateShaderStage> stages;
    std::vector<vk::PipelineShaderStageCreateInfo> vkStages;
};

struct IntermediateVertexInputState {
    IntermediateVertexInputState(const PipelineVertexInputState& a_State)
        : bindings(a_State.bindingDescriptions.size())
        , attributes(a_State.attributeDescriptions.size())
        , info({}, bindings, attributes)
    {
        std::transform(
            a_State.bindingDescriptions.begin(), a_State.bindingDescriptions.end(),
            bindings.begin(), ToVk {});
        std::transform(
            a_State.attributeDescriptions.begin(), a_State.attributeDescriptions.end(),
            attributes.begin(), ToVk {});
    }
    inline auto data() const
    {
        return &info;
    }
    std::vector<vk::VertexInputBindingDescription> bindings;
    std::vector<vk::VertexInputAttributeDescription> attributes;
    vk::PipelineVertexInputStateCreateInfo info;
};
struct IntermediateViewportState {
    IntermediateViewportState(const PipelineViewPortState& a_State)
        : viewports(a_State.viewPorts.size())
        , scissors(a_State.viewPorts.size())
        , info({}, viewports, scissors)
    {
        std::transform(
            a_State.viewPorts.begin(), a_State.viewPorts.end(),
            viewports.begin(), ToVk {});
        std::transform(
            a_State.scissors.begin(), a_State.scissors.end(),
            scissors.begin(), ToVk {});
    }
    inline auto data() const
    {
        return &info;
    }
    std::vector<vk::Viewport> viewports;
    std::vector<vk::Rect2D> scissors;
    vk::PipelineViewportStateCreateInfo info;
};

struct IntermediateColorBlendState {
    IntermediateColorBlendState(const PipelineColorBlendState& a_State)
        : attachments(a_State.attachments.size())
        , info({}, a_State.logicOpEnable, ConvertToVk(a_State.logicOp), attachments)
    {
        std::transform(
            a_State.attachments.begin(), a_State.attachments.end(),
            attachments.begin(), ToVk {});
    }
    inline auto data() const
    {
        return &info;
    }
    std::vector<vk::PipelineColorBlendAttachmentState> attachments;
    vk::PipelineColorBlendStateCreateInfo info;
};

struct IntermediateDynamicState {
    IntermediateDynamicState(const PipelineDynamicState& a_State)
        : states(a_State.dynamicStates.size())
        , info({}, states)
    {
        std::transform(
            a_State.dynamicStates.begin(), a_State.dynamicStates.end(),
            states.begin(), ToVk {});
    }
    inline auto data() const
    {
        return &info;
    }
    std::vector<vk::DynamicState> states;
    vk::PipelineDynamicStateCreateInfo info;
};
}
