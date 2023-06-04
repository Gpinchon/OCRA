/*
 * @Author: gpinchon
 * @Date:   2021-09-26 00:00:00
 * @Last Modified by:   gpinchon
 * @Last Modified time: 2021-09-27 22:56:42
 */
#pragma once

#include <OCRA/Core.hpp>

#include <GL/Enums.hpp>
#include <GL/ExecutionState.hpp>
#include <GL/Structs.hpp>

#include <GL/glew.h>

#include <vector>

namespace OCRA::Pipeline {
static inline auto CompileAttachments(const PipelineColorBlendState& a_Info)
{
    std::vector<GLAttachmentState> attachments;
    attachments.reserve(a_Info.attachments.size());
    for (uint8_t index = 0; index < a_Info.attachments.size(); ++index)
        attachments.push_back(GLAttachmentState(a_Info.attachments.at(index), index));
    return attachments;
}
struct CompileColorBlendState {
    CompileColorBlendState(const Device::Handle& a_Device, const PipelineColorBlendState& a_Info, const PipelineDynamicState& a_DynamicState)
        : logicOpEnable(a_Info.logicOpEnable)
        , logicOp(GetGLLogicOp(a_Info.logicOp))
        , dynamicBlendConstants(a_DynamicState.Contains(DynamicState::BlendConstants))
        , blendConstants(a_Info.blendConstants)
        , attachments(CompileAttachments(a_Info))
    {
    }
    void operator()(Command::Buffer::ExecutionState& a_ExecutionState) const
    {
        logicOpEnable ? glEnable(GL_COLOR_LOGIC_OP) : glDisable(GL_COLOR_LOGIC_OP);
        glLogicOp(logicOp);
        if (dynamicBlendConstants)
            glBlendColor(
                a_ExecutionState.dynamicStates.blendConstants.r,
                a_ExecutionState.dynamicStates.blendConstants.g,
                a_ExecutionState.dynamicStates.blendConstants.b,
                a_ExecutionState.dynamicStates.blendConstants.a);
        else
            glBlendColor(
                blendConstants.r,
                blendConstants.g,
                blendConstants.b,
                blendConstants.a);
        for (const auto& attachment : attachments) {
            attachment.enable ? glEnablei(GL_BLEND, attachment.index) : glDisablei(GL_BLEND, attachment.index);
            glBlendFuncSeparatei(
                attachment.index,
                attachment.srcColorBlendFactor,
                attachment.dstColorBlendFactor,
                attachment.srcAlphaBlendFactor,
                attachment.srcAlphaBlendFactor);
            glBlendEquationSeparatei(
                attachment.index,
                attachment.colorBlendOperation,
                attachment.alphaBlendOperation);
            glColorMaski(
                attachment.index,
                ColorComponentFlagBits::None != (attachment.colorMask & ColorComponentFlagBits::R),
                ColorComponentFlagBits::None != (attachment.colorMask & ColorComponentFlagBits::G),
                ColorComponentFlagBits::None != (attachment.colorMask & ColorComponentFlagBits::B),
                ColorComponentFlagBits::None != (attachment.colorMask & ColorComponentFlagBits::A));
        }
    }
    const bool logicOpEnable;
    const GLenum logicOp;
    const bool dynamicBlendConstants;
    const Vec4 blendConstants;
    const std::vector<GLAttachmentState> attachments;
};
inline auto Default(const Device::Handle& a_Device)
{
    return CompileColorBlendState(a_Device, {}, {});
}
}
