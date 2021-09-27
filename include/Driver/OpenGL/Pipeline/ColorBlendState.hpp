/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-27 22:56:42
*/
#pragma once

#include <Pipeline/ColorBlendState.hpp>

#include <Driver/OpenGL/Blend.hpp>
#include <Driver/OpenGL/Logic.hpp>

#include <GL/glew.h>

#include <array>

namespace OCRA::Pipeline::ColorBlendState {
struct Impl {
    struct AttachmentState {
        AttachmentState() = default;
        AttachmentState(const ColorBlendState::AttachmentState& a_AttachmentState)
            : enable(a_AttachmentState.enable)
            , srcColorBlendFactor(GetGLFactor(a_AttachmentState.srcColorBlendFactor))
            , dstColorBlendFactor(GetGLFactor(a_AttachmentState.dstColorBlendFactor))
            , colorBlendOperation(GetGLOperation(a_AttachmentState.colorBlendOperation))
            , srcAlphaBlendFactor(GetGLFactor(a_AttachmentState.srcAlphaBlendFactor))
            , dstAlphaBlendFactor(GetGLFactor(a_AttachmentState.dstAlphaBlendFactor))
            , alphaBlendOperation(GetGLOperation(a_AttachmentState.alphaBlendOperation))
        {
        }
        bool enable; //is blending enabled ?
        GLenum srcColorBlendFactor;
        GLenum dstColorBlendFactor;
        GLenum colorBlendOperation;
        GLenum srcAlphaBlendFactor;
        GLenum dstAlphaBlendFactor;
        GLenum alphaBlendOperation;
        Blend::ColorMask colorMask;
        void operator()(const Uint8& a_index) const noexcept
        {
            enable ? glEnablei(GL_BLEND, a_index) : glDisablei(GL_BLEND, a_index);
            glBlendFuncSeparatei(
                a_index,
                srcColorBlendFactor,
                dstColorBlendFactor,
                srcAlphaBlendFactor,
                srcAlphaBlendFactor);
            glBlendEquationSeparatei(
                a_index,
                colorBlendOperation,
                alphaBlendOperation);
            glColorMaski(
                a_index,
                Blend::None != (colorMask & Blend::R),
                Blend::None != (colorMask & Blend::G),
                Blend::None != (colorMask & Blend::B),
                Blend::None != (colorMask & Blend::A));
        }
    };
    Impl(const Info& a_Info)
        : logicOpEnable(a_Info.logicOpEnable)
        , logicOp(GetGLOperation(a_Info.logicOp))
        , attachementCount(a_Info.attachementCount)
        , attachments([a_Info] {
            std::array<AttachmentState, FrameBuffer::MaxColorAttachments> attachments;
            for (Uint8 buf = 0; buf < a_Info.attachementCount; ++buf)
                attachments.at(buf) = a_Info.attachments.at(buf);
            return attachments;
        }())
    {
    }
    const bool logicOpEnable;
    const GLenum logicOp;
    const Info::BlendConstants blendConstants;
    const Uint8 attachementCount;
    const std::array<AttachmentState, FrameBuffer::MaxColorAttachments> attachments;
    void operator()(void) const noexcept
    {
        logicOpEnable ? glEnable(GL_COLOR_LOGIC_OP) : glDisable(GL_COLOR_LOGIC_OP);
        glLogicOp(logicOp);
        glBlendColor(
            blendConstants.R,
            blendConstants.G,
            blendConstants.B,
            blendConstants.A);
        for (Uint8 buf = 0; buf < attachementCount; ++buf)
            attachments.at(buf)(buf);
    }
};
//compiles the specified Color Blend State into a callback
inline auto Compile(const Device::Handle& a_Device, const Info& a_Info)
{
    return Impl(a_Info);
}
inline auto Default()
{
    return Impl({});
}
}