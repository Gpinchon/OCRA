#pragma once

#include <OCRA/Core.hpp>

#include <GL/PushConstants.hpp>
#include <GL/Structs.hpp>
#include <GL/Common/Stencil.hpp>
#include <GL/Descriptor/Binding.hpp>
#include <GL/glew.h>

#include <vector>
#include <array>

namespace OCRA::Command::Buffer
{
struct DynamicStates {
    std::vector<ViewPort>   viewPorts;
    std::vector<Rect2D>     scissors;
    Vec<4, GLfloat>         blendConstants{ 0, 0, 0, 0 };
    GLfloat                 lineWidth{ 0.f };
    GLfloat                 depthBiasConstantFactor{ 0.f };
    GLfloat                 depthBiasSlopeFactor{ 0.f };
    GLfloat                 depthBiasClamp{ 0.f };
    DepthBounds<double>     depthBounds{ 0, 0 };
    bool                    depthBiasEnable{ false };
    bool                    depthTestEnable { true };
    bool                    depthWriteEnable { true };
    bool                    depthBoundsTestEnable { false };
    GLenum                  depthCompareOp { GL_LESS };
    bool                    stencilTestEnable { false };
    bool                    rasterizerDiscardEnable { false };
    GLStencilOpState        frontStencilOP;
    GLStencilOpState        backStencilOP;
    GLenum                  primitiveTopology{ GL_NONE };
    bool                    primitiveRestartEnable { false };
    GLenum                  cullMode{ GL_BACK };
    GLenum                  frontFace{ GL_CCW };
};

struct VertexInputBinding {
    OCRA::Buffer::Handle    buffer{ 0 };
    uint64_t                offset{ 0 };
};

struct IndexBufferBinding : VertexInputBinding {
    GLenum type{ GL_NONE };
};

struct RenderPass
{
    void Reset() {
        renderPass.reset();
        framebuffer.reset();
        renderArea = {};
        colorClearValues.clear();
        depthClearValue = 0;
        stencilClearValue = 0;
        vertexInputBindings.clear();
        indexBufferBinding = {};
    }
    OCRA::RenderPass::Handle        renderPass;
    FrameBuffer::Handle             framebuffer;
    Rect2D                          renderArea;
    std::vector<ColorValue>         colorClearValues;
    float                           depthClearValue{ 0 };
    int32_t                         stencilClearValue{ 0 };
    std::vector<VertexInputBinding> vertexInputBindings;
    IndexBufferBinding              indexBufferBinding;
};

using PushDescriptorSet = std::vector<Descriptor::Binding>;

struct PipelineState {
    PipelineState() {
        Reset();
    }
    void Reset() {
        pipeline.reset();
        descriptorDynamicOffset = 0;
        descriptorSet.reset();
        pushDescriptorSet.clear();
    }
    Pipeline::Handle        pipeline;
    Descriptor::Set::Handle descriptorSet;
    PushDescriptorSet       pushDescriptorSet;
    uint32_t                descriptorDynamicOffset;
};

struct ExecutionState {
    ExecutionState()
    {
        Reset();
    }
    ~ExecutionState() {
        Reset();
    }
    void Reset() {
        once = false;
        subpassIndex = uint32_t(-1);
        primitiveTopology = GL_NONE;
        renderPass.Reset();
        dynamicStates = {};
        for (auto& state : pipelineState)
            state.Reset();
        for (auto& state : lastPipelineState)
            state.Reset();
    }

    bool                once{ false };
    uint32_t            subpassIndex{ uint32_t(-1) };
    GLenum              primitiveTopology{ GL_NONE };
    RenderPass          renderPass{};
    DynamicStates       dynamicStates{};
    std::array<PipelineState, size_t(PipelineBindingPoint::MaxValue)> pipelineState{};
    std::array<PipelineState, size_t(PipelineBindingPoint::MaxValue)> lastPipelineState{};
    
};
}