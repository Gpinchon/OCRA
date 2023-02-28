#pragma once

#include <GL/Enums.hpp>

namespace OCRA
{
struct GLAttachmentState {
    GLAttachmentState(const PipelineColorBlendAttachmentState& a_AttachmentState, const uint8_t& a_Index)
        : index(a_Index)
        , enable(a_AttachmentState.enable)
        , srcColorBlendFactor(GetGLBlendFactor(a_AttachmentState.srcColorBlendFactor))
        , dstColorBlendFactor(GetGLBlendFactor(a_AttachmentState.dstColorBlendFactor))
        , colorBlendOperation(GetGLBlendOp(a_AttachmentState.colorBlendOperation))
        , srcAlphaBlendFactor(GetGLBlendFactor(a_AttachmentState.srcAlphaBlendFactor))
        , dstAlphaBlendFactor(GetGLBlendFactor(a_AttachmentState.dstAlphaBlendFactor))
        , alphaBlendOperation(GetGLBlendOp(a_AttachmentState.alphaBlendOperation))
        , colorMask(a_AttachmentState.colorMask)
    {}
    bool enable; //is blending enabled ?
    GLenum srcColorBlendFactor;
    GLenum dstColorBlendFactor;
    GLenum colorBlendOperation;
    GLenum srcAlphaBlendFactor;
    GLenum dstAlphaBlendFactor;
    GLenum alphaBlendOperation;
    BlendColorMaskFlags colorMask; //color mask used for writing to this attachment
    const uint8_t index{ 0 };
};

struct GLRasterizationState {
    GLRasterizationState(const PipelineRasterizationState& a_Info)
    : rasterizerDiscardEnable(a_Info.rasterizerDiscardEnable)
    , depthClampEnable(a_Info.depthClampEnable)
    , depthBiasEnable(a_Info.depthBiasEnable)
    , depthBiasConstantFactor(a_Info.depthBiasConstantFactor)
    , depthBiasClamp(a_Info.depthBiasClamp)
    , depthBiasSlopeFactor(a_Info.depthBiasSlopeFactor)
    , lineWidth(a_Info.lineWidth)
    , polygonOffsetMode(GetGLPolygonOffsetMode(a_Info.polygonMode))
    , polygonMode(GetGLPolygonMode(a_Info.polygonMode))
    , cullMode(GetGLCullMode(a_Info.cullMode))
    , frontFace(GetGLFrontFace(a_Info.frontFace))
    {}
    GLboolean rasterizerDiscardEnable { false };
    GLboolean depthClampEnable { true };
    GLboolean depthBiasEnable { false };
    GLfloat depthBiasConstantFactor { 0 };
    GLfloat depthBiasClamp { 0 };
    GLfloat depthBiasSlopeFactor { 0 };
    GLfloat lineWidth { 1 };
    GLenum polygonOffsetMode{ GL_POLYGON_OFFSET_FILL };
    GLenum polygonMode { GL_FILL };
    GLenum cullMode { GL_BACK };
    GLenum frontFace { GL_CCW };
};

struct GLStencilOpState {
    GLStencilOpState(const PipelineStencilOpState& a_OpState = {})
    : failOp(GetGLOperation(a_OpState.failOp))
    , passOp(GetGLOperation(a_OpState.passOp))
    , depthFailOp(GetGLOperation(a_OpState.depthFailOp))
    , compareOp(GetGLOperation(a_OpState.compareOp))
    , compareMask(a_OpState.compareMask)
    , writeMask(a_OpState.writeMask)
    , reference(a_OpState.reference)
    {}
    GLenum failOp; //the operation to be realized when stencil test FAILS
    GLenum passOp; //the operation to be realized when stencil test PASSES
    GLenum depthFailOp; //the operation to be realized when stencil test PASSES but depth test FAILS
    GLenum compareOp;
    uint32_t compareMask; //a mask that is ANDed with ref and the buffer's content
    uint32_t writeMask; //a mask that is ANDed with the stencil value about to be written to the buffer
    uint32_t reference; //the reference value used in comparison.
};

struct GLSwizzleMask {
    GLSwizzleMask(const ComponentMapping& a_Mapping)
        : r(GetGLSwizzle(a_Mapping.r, GL_TEXTURE_SWIZZLE_R))
        , g(GetGLSwizzle(a_Mapping.g, GL_TEXTURE_SWIZZLE_G))
        , b(GetGLSwizzle(a_Mapping.b, GL_TEXTURE_SWIZZLE_B))
        , a(GetGLSwizzle(a_Mapping.a, GL_TEXTURE_SWIZZLE_A))
    {}
    const GLenum r;
    const GLenum g;
    const GLenum b;
    const GLenum a;
};
}