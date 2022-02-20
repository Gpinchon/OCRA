/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-27 22:56:42
*/
#pragma once

#include <Pipeline/ColorBlendState.hpp>

#include <GL/Blend.hpp>
#include <GL/Logic.hpp>
#include <GL/glew.h>

#include <functional>
#include <array>

namespace OCRA::Pipeline::ColorBlendState {
inline const auto Compile(const ColorBlendState::AttachmentState& a_AttachmentState, const Uint8& a_Index)
{
    return [
        index(a_Index),
        enable(a_AttachmentState.enable),
        srcColorBlendFactor(GetGLFactor(a_AttachmentState.srcColorBlendFactor)),
        dstColorBlendFactor(GetGLFactor(a_AttachmentState.dstColorBlendFactor)),
        colorBlendOperation(GetGLOperation(a_AttachmentState.colorBlendOperation)),
        srcAlphaBlendFactor(GetGLFactor(a_AttachmentState.srcAlphaBlendFactor)),
        dstAlphaBlendFactor(GetGLFactor(a_AttachmentState.dstAlphaBlendFactor)),
        alphaBlendOperation(GetGLOperation(a_AttachmentState.alphaBlendOperation)),
        colorMask(a_AttachmentState.colorMask)
    ]() {
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
    };
}
inline const auto Compile(const Device::Handle& a_Device, const Info& a_Info) {
    const auto attachments([a_Info] {
        std::array<std::function<void()>, FrameBuffer::MaxColorAttachments> attachments;
        for (Uint8 buf = 0; buf < a_Info.attachementCount; ++buf)
            attachments.at(buf) = Compile(a_Info.attachments.at(buf), buf);
        return attachments;
    }());
    return [
        logicOpEnable(a_Info.logicOpEnable),
        logicOp(GetGLOperation(a_Info.logicOp)),
        blendConstants(a_Info.blendConstants),
        attachementCount(a_Info.attachementCount),
        attachments(attachments)
    ]() {
        logicOpEnable ? glEnable(GL_COLOR_LOGIC_OP) : glDisable(GL_COLOR_LOGIC_OP);
        glLogicOp(logicOp);
        glBlendColor(
            blendConstants.R,
            blendConstants.G,
            blendConstants.B,
            blendConstants.A);
        for (Uint8 buf = 0; buf < attachementCount; ++buf) attachments.at(buf)();
    };
}
inline auto Default(const Device::Handle& a_Device)
{
    return Compile(a_Device, {});
}
}