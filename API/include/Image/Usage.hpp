#pragma once

#include <bitset>

namespace OCRA::Image
{
using UsageFlags = std::bitset<8>;
namespace UsageFlagBits {
constexpr UsageFlags TransferSrc             = 0b10000000;
constexpr UsageFlags TransferDst             = 0b01000000;
constexpr UsageFlags Sampled                 = 0b00100000;
constexpr UsageFlags Storage                 = 0b00010000;
constexpr UsageFlags ColorAttachment         = 0b00001000;
constexpr UsageFlags DepthStencilAttachment  = 0b00000100;
constexpr UsageFlags TransientAttachment     = 0b00000010;
constexpr UsageFlags InputAttachment         = 0b00000001;
//VK_VERSION_1_3
constexpr UsageFlags None                    = 0b00000000;
}
}