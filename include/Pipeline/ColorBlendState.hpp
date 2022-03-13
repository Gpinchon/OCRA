/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:28:32
*/
#pragma once

#include <Common/Vec4.hpp>
#include <Common/Blend.hpp>
#include <Common/Logic.hpp>
#include <FrameBuffer.hpp>

#include <array>

namespace OCRA::Pipeline::ColorBlendState {
struct AttachmentState {
    bool enable { false }; //is blending enabled ?
    Blend::Factor srcColorBlendFactor { Blend::Factor::One };
    Blend::Factor dstColorBlendFactor { Blend::Factor::Zero };
    Blend::Operation colorBlendOperation { Blend::Operation::Add };
    Blend::Factor srcAlphaBlendFactor { Blend::Factor::One };
    Blend::Factor dstAlphaBlendFactor { Blend::Factor::Zero };
    Blend::Operation alphaBlendOperation { Blend::Operation::Add };
    Blend::ColorMask colorMask { Blend::R | Blend::G | Blend::B | Blend::A }; //color mask used for writing to this attachment
};
struct Info {
    bool logicOpEnable{ false };
    Logic::Operation logicOp{ Logic::Operation::Copy }; //see glLogicOp
    uint8_t attachementCount { 0 };
    std::array<AttachmentState, FrameBuffer::MaxColorAttachments> attachments;
    Vec4 blendConstants;
};
}