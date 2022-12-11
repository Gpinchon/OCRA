/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 15:03:53
*/
#include <OCRA/Pipeline/Pipeline.hpp>
#include <OCRA/Pipeline/Graphics.hpp>

#include <GL/Pipeline/Pipeline.hpp>
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

#include <cassert>

namespace OCRA::Pipeline::Graphics {
struct Impl : Pipeline::Impl {
    Impl(const Device::Handle& a_Device, const Info& a_Info)
        : Pipeline::Impl(BindingPoint::Graphics, a_Info.layout)
        , renderPass(a_Info.renderPass)
        , subPass(a_Info.subPass)
        , colorBlendState(ColorBlendState::Compile(a_Device, a_Info.colorBlendState, a_Info.dynamicState))
        , depthStencilState(DepthStencilState::Compile(a_Device, a_Info.depthStencilState, a_Info.dynamicState))
        , inputAssemblyState(InputAssemblyState::Compile(a_Device, a_Info.inputAssemblyState, a_Info.dynamicState))
        , multisampleState(MultisampleState::Compile(a_Device, a_Info.multisampleState, a_Info.dynamicState))
        , rasterizationState(RasterizationState::Compile(a_Device, a_Info.rasterizationState, a_Info.dynamicState))
        , shaderPipelineState(ShaderPipelineState::Compile(a_Device, a_Info.shaderPipelineState, a_Info.dynamicState))
        , tessellationState(TessellationState::Compile(a_Device, a_Info.tessellationState, a_Info.dynamicState))
        , viewportState(ViewPortState::Compile(a_Device, a_Info.viewPortState, a_Info.dynamicState))
        , vertexInputState(VertexInputState::Compile(a_Device, a_Info.vertexInputState, a_Info.dynamicState))
    {}
    virtual void Apply(Command::Buffer::ExecutionState& a_ExecutionState) const override {
        assert(renderPass == a_ExecutionState.renderPass.renderPass);
        if (a_ExecutionState.subpassIndex != subPass) {
            a_ExecutionState.subpassIndex = subPass;
            a_ExecutionState.renderPass.renderPass->BeginSubPass(a_ExecutionState);
        }
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
    const uint32_t                      subPass;
    const RenderPass::Handle            renderPass;
    const ColorBlendState::Compile      colorBlendState;
    const DepthStencilState::Compile    depthStencilState;
    const InputAssemblyState::Compile   inputAssemblyState;
    const MultisampleState::Compile     multisampleState;
    const RasterizationState::Compile   rasterizationState;
    const ShaderPipelineState::Compile  shaderPipelineState;
    const TessellationState::Compile    tessellationState;
    const ViewPortState::Compile        viewportState;
    const VertexInputState::Compile     vertexInputState;
};
Handle Create(const Device::Handle& a_Device, const Info& a_Info) {
    return Handle(new Graphics::Impl(a_Device, a_Info));
}
}