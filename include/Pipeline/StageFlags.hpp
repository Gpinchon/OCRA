#pragma once

#include <bitset>

namespace OCRA::Pipeline::StageFlags
{
using Bits = std::bitset<32>;
constexpr Bits TopOfPipe                    = 0b00000000000000000000000000000001;
constexpr Bits DrawIndirect                 = 0b00000000000000000000000000000010;
constexpr Bits VertexInput                  = 0b00000000000000000000000000000100;
constexpr Bits VertexShader                 = 0b00000000000000000000000000001000;
constexpr Bits TessellationControlShader    = 0b00000000000000000000000000010000;
constexpr Bits TessellationEvaluationShader = 0b00000000000000000000000000100000;
constexpr Bits GeometryShader               = 0b00000000000000000000000001000000;
constexpr Bits FragmentShader               = 0b00000000000000000000000010000000;
constexpr Bits EarlyFragmentTests           = 0b00000000000000000000000100000000;
constexpr Bits LateFragmentTests            = 0b00000000000000000000001000000000;
constexpr Bits ColorAttachmentOutput        = 0b00000000000000000000010000000000;
constexpr Bits ComputeShader                = 0b00000000000000000000100000000000;
constexpr Bits Transfer                     = 0b00000000000000000001000000000000;
constexpr Bits BottomOfPipe                 = 0b00000000000000000010000000000000;
constexpr Bits Host                         = 0b00000000000000000100000000000000;
constexpr Bits AllGraphics                  = 0b00000000000000001000000000000000;
constexpr Bits AllCommands                  = 0b00000000000000010000000000000000;
//VK 1.3
constexpr Bits None                         = 0b00000000000000000000000000000000;
}