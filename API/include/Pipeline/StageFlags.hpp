#pragma once

#include <bitset>

namespace OCRA::Pipeline
{
using StageFlags = std::bitset<32>;
namespace StageFlagBits {
constexpr StageFlags TopOfPipe                    = 0b00000000000000000000000000000001;
constexpr StageFlags DrawIndirect                 = 0b00000000000000000000000000000010;
constexpr StageFlags VertexInput                  = 0b00000000000000000000000000000100;
constexpr StageFlags VertexShader                 = 0b00000000000000000000000000001000;
constexpr StageFlags TessellationControlShader    = 0b00000000000000000000000000010000;
constexpr StageFlags TessellationEvaluationShader = 0b00000000000000000000000000100000;
constexpr StageFlags GeometryShader               = 0b00000000000000000000000001000000;
constexpr StageFlags FragmentShader               = 0b00000000000000000000000010000000;
constexpr StageFlags EarlyFragmentTests           = 0b00000000000000000000000100000000;
constexpr StageFlags LateFragmentTests            = 0b00000000000000000000001000000000;
constexpr StageFlags ColorAttachmentOutput        = 0b00000000000000000000010000000000;
constexpr StageFlags ComputeShader                = 0b00000000000000000000100000000000;
constexpr StageFlags Transfer                     = 0b00000000000000000001000000000000;
constexpr StageFlags BottomOfPipe                 = 0b00000000000000000010000000000000;
constexpr StageFlags Host                         = 0b00000000000000000100000000000000;
constexpr StageFlags AllGraphics                  = 0b00000000000000001000000000000000;
constexpr StageFlags AllCommands                  = 0b00000000000000010000000000000000;
//VK 1.3
constexpr StageFlags None                         = 0b00000000000000000000000000000000;
}
}