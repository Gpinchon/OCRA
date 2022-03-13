/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-27 22:56:42
*/
#pragma once

#include <Pipeline/ColorBlendState.hpp>
#include <Pipeline/DynamicState.hpp>

#include <GL/Command/ExecutionState.hpp>
#include <GL/Common/Blend.hpp>
#include <GL/Common/Logic.hpp>
#include <GL/glew.h>

#include <functional>
#include <vector>

namespace OCRA::Pipeline::ColorBlendState {
struct GLAttachmentState {
    GLAttachmentState(const AttachmentState& a_AttachmentState = {})
    : enable(a_AttachmentState.enable)
    , srcColorBlendFactor(GetGLFactor(a_AttachmentState.srcColorBlendFactor))
    , dstColorBlendFactor(GetGLFactor(a_AttachmentState.dstColorBlendFactor))
    , colorBlendOperation(GetGLOperation(a_AttachmentState.colorBlendOperation))
    , srcAlphaBlendFactor(GetGLFactor(a_AttachmentState.srcAlphaBlendFactor))
    , dstAlphaBlendFactor(GetGLFactor(a_AttachmentState.dstAlphaBlendFactor))
    , alphaBlendOperation(GetGLOperation(a_AttachmentState.alphaBlendOperation))
    , colorMask(a_AttachmentState.colorMask)
    {}
    bool enable; //is blending enabled ?
    GLenum srcColorBlendFactor;
    GLenum dstColorBlendFactor;
    GLenum colorBlendOperation;
    GLenum srcAlphaBlendFactor;
    GLenum dstAlphaBlendFactor;
    GLenum alphaBlendOperation;
    Blend::ColorMask colorMask; //color mask used for writing to this attachment
};
inline const auto Compile(const AttachmentState& a_AttachmentState, const uint8_t& a_Index)
{
    return [
        index(a_Index),
        attachmentState(GLAttachmentState(a_AttachmentState))
    ]() {
        attachmentState.enable ? glEnablei(GL_BLEND, index) : glDisablei(GL_BLEND, index);
        glBlendFuncSeparatei(
            index,
            attachmentState.srcColorBlendFactor,
            attachmentState.dstColorBlendFactor,
            attachmentState.srcAlphaBlendFactor,
            attachmentState.srcAlphaBlendFactor);
        glBlendEquationSeparatei(
            index,
            attachmentState.colorBlendOperation,
            attachmentState.alphaBlendOperation);
        glColorMaski(
            index,
            Blend::None != (attachmentState.colorMask & Blend::R),
            Blend::None != (attachmentState.colorMask & Blend::G),
            Blend::None != (attachmentState.colorMask & Blend::B),
            Blend::None != (attachmentState.colorMask & Blend::A));
    };
}
inline const std::function<void(Command::Buffer::ExecutionState&)> Compile(const Device::Handle& a_Device, const Info& a_Info, const DynamicState::Info& a_DynamicState) {
    const auto attachments = [a_Info] {
        std::vector<std::function<void()>> attachments;
        attachments.reserve(a_Info.attachementCount);
        for (uint8_t index = 0; index < a_Info.attachementCount; ++index)
            attachments.push_back(Compile(a_Info.attachments.at(index), index));
        return attachments;
    }();
    if (a_DynamicState.Contains(DynamicState::State::BlendConstants)) //use dynamic blend constants
        return [
            logicOpEnable(a_Info.logicOpEnable),
            logicOp(GetGLOperation(a_Info.logicOp)),
            attachments(attachments)
        ](Command::Buffer::ExecutionState& a_ExecutionState) {
            logicOpEnable ? glEnable(GL_COLOR_LOGIC_OP) : glDisable(GL_COLOR_LOGIC_OP);
            glLogicOp(logicOp);
            glBlendColor(
                a_ExecutionState.dynamicStates.blendConstants.r,
                a_ExecutionState.dynamicStates.blendConstants.g,
                a_ExecutionState.dynamicStates.blendConstants.b,
                a_ExecutionState.dynamicStates.blendConstants.a);
            for (const auto& attachment : attachments) attachment();
        };
    else return [
            logicOpEnable(a_Info.logicOpEnable),
            logicOp(GetGLOperation(a_Info.logicOp)),
            blendConstants(a_Info.blendConstants),
            attachments(attachments)
        ](Command::Buffer::ExecutionState&) {
            logicOpEnable ? glEnable(GL_COLOR_LOGIC_OP) : glDisable(GL_COLOR_LOGIC_OP);
            glLogicOp(logicOp);
            glBlendColor(
                blendConstants.r,
                blendConstants.g,
                blendConstants.b,
                blendConstants.a);
            for (const auto& attachment : attachments) attachment();
        };
}
inline auto Default(const Device::Handle& a_Device)
{
    return Compile(a_Device, {}, {});
}
}