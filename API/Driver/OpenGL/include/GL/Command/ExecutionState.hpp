#pragma once

#include <OCRA/Handle.hpp>
#include <OCRA/Common/ClearValue.hpp>
#include <OCRA/Common/Compare.hpp>
#include <OCRA/Common/DepthBounds.hpp>
#include <OCRA/Common/Stencil.hpp>
#include <OCRA/Common/Rect2D.hpp>
#include <OCRA/Common/Vec4.hpp>

#include <OCRA/Common/ViewPort.hpp>
#include <OCRA/FrameBuffer.hpp>
#include <OCRA/RenderPass.hpp>

#include <GL/Common/Stencil.hpp>
#include <GL/Descriptor/SetBinding.hpp>
#include <GL/PushConstants.hpp>
#include <GL/glew.h>

#include <vector>
#include <array>
#include <algorithm>

OCRA_DECLARE_HANDLE(OCRA::Pipeline);
OCRA_DECLARE_HANDLE(OCRA::Buffer);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::Set);

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
    Stencil::GLOpState      frontStencilOP;
    Stencil::GLOpState      backStencilOP;
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
    OCRA::RenderPass::Handle        renderPass;
    FrameBuffer::Handle             framebuffer;
    Rect2D                          renderArea;
    std::vector<ColorValue>         colorClearValues;
    float                           depthClearValue{ 0 };
    int32_t                         stencilClearValue{ 0 };
    std::vector<VertexInputBinding> vertexInputBindings;
    IndexBufferBinding              indexBufferBinding;
};

struct PushDescriptorSet {
    std::vector<Descriptor::Set::Binding>   bindings;
};

struct PipelineState {
    Pipeline::Handle pipeline;
    std::array<PushDescriptorSet, 4>        pushDescriptorSets;
    std::array<Descriptor::Set::Handle, 4>  descriptorSets{};
    std::array<uint32_t, 4>                 descriptorDynamicOffsets{ 0, 0, 0, 0 };
};

struct ExecutionState {
    ExecutionState(const Device::Handle& a_Device)
        : pushConstants(a_Device)
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
        renderPass = {};
        dynamicStates = {};

        pipelineState.fill({});
        lastPipelineState.fill({});
    }

    bool                once{ false };
    uint32_t            subpassIndex{ uint32_t(-1) };
    GLenum              primitiveTopology{ GL_NONE };
    RenderPass          renderPass{};
    DynamicStates       dynamicStates{};
    OCRA::PushConstants pushConstants;
    std::array<PipelineState, size_t(Pipeline::BindingPoint::MaxValue)>         pipelineState{};
    std::array<PipelineState, size_t(Pipeline::BindingPoint::MaxValue)>         lastPipelineState{};
    
};
}