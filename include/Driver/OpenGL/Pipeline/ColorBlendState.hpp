/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 16:44:02
*/
#pragma once

#include <Pipeline/ColorBlendState.hpp>

#include <GL/glew.h>
#include <Driver/OpenGL/Blend.hpp>
#include <Driver/OpenGL/Logic.hpp>

#include <functional>
#include <stdexcept>
#include <array>

namespace OCRA::Pipeline::ColorBlendState {
struct Impl {
    struct AttachmentState {
        AttachmentState() = default;
        AttachmentState(const ColorBlendState::AttachmentState& a_AttachmentState)
            : enable(a_AttachmentState.enable)
            , srcColorBlendFactor(GetGLBlendFactor(a_AttachmentState.srcColorBlendFactor))
            , dstColorBlendFactor(GetGLBlendFactor(a_AttachmentState.dstColorBlendFactor))
            , colorBlendOperation(GetGLBlendOperation(a_AttachmentState.colorBlendOperation))
            , srcAlphaBlendFactor(GetGLBlendFactor(a_AttachmentState.srcAlphaBlendFactor))
            , dstAlphaBlendFactor(GetGLBlendFactor(a_AttachmentState.dstAlphaBlendFactor))
            , alphaBlendOperation(GetGLBlendOperation(a_AttachmentState.alphaBlendOperation))
        {
        }
        bool enable { false }; //is blending enabled ?
        GLenum srcColorBlendFactor { GL_ONE };
        GLenum dstColorBlendFactor { GL_ZERO };
        GLenum colorBlendOperation { GL_FUNC_ADD };
        GLenum srcAlphaBlendFactor { GL_ONE };
        GLenum dstAlphaBlendFactor { GL_ZERO };
        GLenum alphaBlendOperation { GL_FUNC_ADD };
        Blend::ColorMask colorMask { Blend::R | Blend::G | Blend::B | Blend::A };
    };
    Impl() {};
    Impl(const Info& a_Info)
        : logicOpEnable(a_Info.logicOpEnable)
        , logicOp(GetGLLogicOperation(a_Info.logicOp))
        , attachementCount(a_Info.attachementCount)
        , attachments([a_Info]{
        std::array<AttachmentState, FrameBuffer::MaxColorAttachments> attachments;
        for (Uint8 buf = 0; buf < a_Info.attachementCount; ++buf)
            attachments.at(buf) = a_Info.attachments.at(buf);
        return attachments;
        }())
    {
    }
    const bool logicOpEnable { false };
    const GLenum logicOp { GL_COPY };
    const Info::BlendConstants blendConstants;
    const Uint8 attachementCount { 0 };
    const std::array<AttachmentState, FrameBuffer::MaxColorAttachments> attachments{};
    void operator()(void) const
    {
        if (logicOpEnable) {
            glEnable(GL_COLOR_LOGIC_OP);
            glLogicOp(logicOp);
        }
        else
            glDisable(GL_COLOR_LOGIC_OP);
        glBlendColor(
            blendConstants.R,
            blendConstants.G,
            blendConstants.B,
            blendConstants.A);
        for (Uint8 buf = 0; buf < attachementCount; ++buf) {
            if (attachments[buf].enable)
                glEnablei(GL_BLEND, buf);
            else {
                glDisablei(GL_BLEND, buf);
                continue;
            }
            glBlendFuncSeparatei(
                buf,
                attachments[buf].srcColorBlendFactor,
                attachments[buf].dstColorBlendFactor,
                attachments[buf].srcAlphaBlendFactor,
                attachments[buf].srcAlphaBlendFactor);
            glBlendEquationSeparatei(
                buf,
                attachments[buf].colorBlendOperation,
                attachments[buf].alphaBlendOperation);
            glColorMaski(
                buf,
                Blend::None != (attachments[buf].colorMask & Blend::R),
                Blend::None != (attachments[buf].colorMask & Blend::G),
                Blend::None != (attachments[buf].colorMask & Blend::B),
                Blend::None != (attachments[buf].colorMask & Blend::A));
        }
    }
};
//compiles the specified Color Blend State into a callback
inline auto Compile(const Info& a_Info)
{
    return Impl(a_Info);
}
inline auto Default()
{
    return Impl();
}
}