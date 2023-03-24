/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 15:03:53
*/
#include <OCRA/Core.hpp>

#include <GL/ExecutionState.hpp>
#include <GL/Common/Assert.hpp>
#include <GL/Pipeline.hpp>
#include <GL/Pipeline/ColorBlendState.hpp>
#include <GL/Pipeline/DepthStencilState.hpp>
#include <GL/Pipeline/InputAssemblyState.hpp>
#include <GL/Pipeline/MultisampleState.hpp>
#include <GL/Pipeline/RasterizationState.hpp>
#include <GL/Pipeline/ShaderPipelineState.hpp>
#include <GL/Pipeline/TessellationState.hpp>
#include <GL/Pipeline/ViewPortState.hpp>
#include <GL/Pipeline/VertexInputState.hpp>
#include <GL/RenderPass.hpp>

namespace OCRA::Pipeline::Graphics {
struct Impl : Pipeline::Impl {
    Impl::Impl(const Device::Handle& a_Device, const CreatePipelineGraphicsInfo& a_Info);
    virtual void Impl::Apply(Command::Buffer::ExecutionState& a_ExecutionState) const override;
    //const uint32_t                      subPass;
    //const RenderPass::Handle            renderPass;
    const CompileColorBlendState      colorBlendState;
    const CompileDepthStencilState    depthStencilState;
    const CompileInputAssemblyState   inputAssemblyState;
    const CompileMultisampleState     multisampleState;
    const CompileRasterizationState   rasterizationState;
    const CompileShaderPipelineState  shaderPipelineState;
    const CompileTessellationState    tessellationState;
    const CompileViewPortState        viewportState;
    const CompileVertexInputState     vertexInputState;
};

Impl::Impl(const Device::Handle& a_Device, const CreatePipelineGraphicsInfo& a_Info)
    : Pipeline::Impl(PipelineBindingPoint::Graphics, a_Info.layout)
    //, renderPass(a_Info.renderPass)
    //, subPass(a_Info.subPass)
    , colorBlendState(a_Device, a_Info.colorBlendState, a_Info.dynamicState)
    , depthStencilState(a_Device, a_Info.depthStencilState, a_Info.dynamicState)
    , inputAssemblyState(a_Device, a_Info.inputAssemblyState, a_Info.dynamicState)
    , multisampleState(a_Device, a_Info.multisampleState, a_Info.dynamicState)
    , rasterizationState(a_Device, a_Info.rasterizationState, a_Info.dynamicState)
    , shaderPipelineState(a_Device, a_Info.shaderPipelineState, a_Info.dynamicState)
    , tessellationState(a_Device, a_Info.tessellationState, a_Info.dynamicState)
    , viewportState(a_Device, a_Info.viewPortState, a_Info.dynamicState)
    , vertexInputState(a_Device, a_Info.vertexInputState, a_Info.dynamicState)
{}

void Impl::Apply(Command::Buffer::ExecutionState& a_ExecutionState) const {
    //OCRA_ASSERT(renderPass == a_ExecutionState.renderPass.renderPass);
    //if (a_ExecutionState.subpassIndex != subPass) {
    //    a_ExecutionState.subpassIndex = subPass;
    //    a_ExecutionState.renderPass.renderPass->BeginSubPass(a_ExecutionState);
    //}
    const auto& lastPipeline = std::static_pointer_cast<Impl>(a_ExecutionState.lastPipelineState.at(size_t(PipelineBindingPoint::Graphics)).pipeline);
    if (lastPipeline.get() == this) return; //we did not change states set
    if (lastPipeline == nullptr) { //first graphics pipeline on this command buffer, just apply the states
        colorBlendState(a_ExecutionState);
        depthStencilState(a_ExecutionState);
        inputAssemblyState(a_ExecutionState);
        multisampleState(a_ExecutionState);
        rasterizationState(a_ExecutionState);
        shaderPipelineState(a_ExecutionState);
        tessellationState(a_ExecutionState);
        viewportState(a_ExecutionState);
        vertexInputState(a_ExecutionState);
    }
    else {
        if (std::memcmp(&lastPipeline->colorBlendState, &colorBlendState, sizeof(colorBlendState)) != 0)
            colorBlendState(a_ExecutionState);
        if (std::memcmp(&lastPipeline->depthStencilState, &depthStencilState, sizeof(depthStencilState)) != 0)
            depthStencilState(a_ExecutionState);
        if (std::memcmp(&lastPipeline->inputAssemblyState, &inputAssemblyState, sizeof(inputAssemblyState)) != 0)
            inputAssemblyState(a_ExecutionState);
        if (std::memcmp(&lastPipeline->multisampleState, &multisampleState, sizeof(multisampleState)) != 0)
            multisampleState(a_ExecutionState);
        if (std::memcmp(&lastPipeline->rasterizationState, &rasterizationState, sizeof(rasterizationState)) != 0)
            rasterizationState(a_ExecutionState);
        if (std::memcmp(&lastPipeline->shaderPipelineState, &shaderPipelineState, sizeof(shaderPipelineState)) != 0)
            shaderPipelineState(a_ExecutionState);
        if (std::memcmp(&lastPipeline->tessellationState, &tessellationState, sizeof(tessellationState)) != 0)
            tessellationState(a_ExecutionState);
        if (std::memcmp(&lastPipeline->viewportState, &viewportState, sizeof(viewportState)) != 0)
            viewportState(a_ExecutionState);
        if (std::memcmp(&lastPipeline->vertexInputState, &vertexInputState, sizeof(vertexInputState)) != 0)
            vertexInputState(a_ExecutionState);
    }
}
}

namespace OCRA::Device
{
Pipeline::Handle CreatePipelineGraphics(const Device::Handle& a_Device, const CreatePipelineGraphicsInfo& a_Info) {
    return std::make_shared<Pipeline::Graphics::Impl>(a_Device, a_Info);
}
}