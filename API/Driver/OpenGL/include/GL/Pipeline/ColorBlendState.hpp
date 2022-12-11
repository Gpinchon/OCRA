/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-27 22:56:42
*/
#pragma once

#include <OCRA/Pipeline/ColorBlendState.hpp>
#include <OCRA/Pipeline/DynamicState.hpp>

#include <GL/Command/ExecutionState.hpp>
#include <GL/Common/Blend.hpp>
#include <GL/Common/Logic.hpp>
#include <GL/glew.h>

#include <functional>
#include <vector>

namespace OCRA::Pipeline::ColorBlendState {
struct GLAttachmentState {
    GLAttachmentState(const AttachmentState& a_AttachmentState, const uint8_t& a_Index)
        : index(a_Index)
        , enable(a_AttachmentState.enable)
        , srcColorBlendFactor(GetGLFactor(a_AttachmentState.srcColorBlendFactor))
        , dstColorBlendFactor(GetGLFactor(a_AttachmentState.dstColorBlendFactor))
        , colorBlendOperation(GetGLOperation(a_AttachmentState.colorBlendOperation))
        , srcAlphaBlendFactor(GetGLFactor(a_AttachmentState.srcAlphaBlendFactor))
        , dstAlphaBlendFactor(GetGLFactor(a_AttachmentState.dstAlphaBlendFactor))
        , alphaBlendOperation(GetGLOperation(a_AttachmentState.alphaBlendOperation))
        , colorMask(a_AttachmentState.colorMask)
    {}
    void operator()() const {
        enable ? glEnablei(GL_BLEND, index) : glDisablei(GL_BLEND, index);
        glBlendFuncSeparatei(
            index,
            srcColorBlendFactor,
            dstColorBlendFactor,
            srcAlphaBlendFactor,
            srcAlphaBlendFactor);
        glBlendEquationSeparatei(
            index,
            colorBlendOperation,
            alphaBlendOperation);
        glColorMaski(
            index,
            Blend::None != (colorMask & Blend::R),
            Blend::None != (colorMask & Blend::G),
            Blend::None != (colorMask & Blend::B),
            Blend::None != (colorMask & Blend::A));
    }
    bool enable; //is blending enabled ?
    GLenum srcColorBlendFactor;
    GLenum dstColorBlendFactor;
    GLenum colorBlendOperation;
    GLenum srcAlphaBlendFactor;
    GLenum dstAlphaBlendFactor;
    GLenum alphaBlendOperation;
    Blend::ColorMask colorMask; //color mask used for writing to this attachment
    const uint8_t index{ 0 };
};
static inline auto CompileAttachments(const Info& a_Info) {
    std::vector<GLAttachmentState> attachments;
    attachments.reserve(a_Info.attachments.size());
    for (uint8_t index = 0; index < a_Info.attachments.size(); ++index)
        attachments.push_back(GLAttachmentState(a_Info.attachments.at(index), index));
    return attachments;
}
struct Compile {
    Compile(const Device::Handle& a_Device, const Info& a_Info, const DynamicState::Info& a_DynamicState)
        : logicOpEnable(a_Info.logicOpEnable)
        , logicOp(GetGLOperation(a_Info.logicOp))
        , dynamicBlendConstants(a_DynamicState.Contains(DynamicState::State::BlendConstants))
        , blendConstants(a_Info.blendConstants)
        , attachments(CompileAttachments(a_Info))
    {}
    void operator()(Command::Buffer::ExecutionState& a_ExecutionState) const {
        logicOpEnable ? glEnable(GL_COLOR_LOGIC_OP) : glDisable(GL_COLOR_LOGIC_OP);
        glLogicOp(logicOp);
        if (dynamicBlendConstants)
            glBlendColor(
                a_ExecutionState.dynamicStates.blendConstants.r,
                a_ExecutionState.dynamicStates.blendConstants.g,
                a_ExecutionState.dynamicStates.blendConstants.b,
                a_ExecutionState.dynamicStates.blendConstants.a);
        else glBlendColor(
            blendConstants.r,
            blendConstants.g,
            blendConstants.b,
            blendConstants.a);
        for (const auto& attachment : attachments) attachment();
    }
    const bool logicOpEnable;
    const GLenum logicOp;
    const bool dynamicBlendConstants;
    const Vec4 blendConstants;
    const std::vector<GLAttachmentState> attachments;
};
inline auto Default(const Device::Handle& a_Device)
{
    return Compile(a_Device, {}, {});
}
}