#pragma once

#include <Handle.hpp>
#include <Pipeline/StageFlags.hpp>

#include <vector>
#include <bitset>

OCRA_DECLARE_HANDLE(OCRA::Command::Buffer);
OCRA_DECLARE_HANDLE(OCRA::Buffer);
OCRA_DECLARE_HANDLE(OCRA::Image);

namespace OCRA::Command {
using DependencyFlags = std::bitset<3>;
namespace DependencyFlagBits {
constexpr DependencyFlags ByRegion    = 0b001;
constexpr DependencyFlags ViewLocal   = 0b010;
constexpr DependencyFlags DeviceGroup = 0b100;
constexpr DependencyFlags None        = 0b000;
}
using AccessFlags = std::bitset<17>;
namespace AccessFlagBits {
constexpr AccessFlags IndirectCommandRead           = 0b10000000000000000;
constexpr AccessFlags IndexRead                     = 0b01000000000000000;
constexpr AccessFlags VertexAttributeRead           = 0b00100000000000000;
constexpr AccessFlags UniformRead                   = 0b00010000000000000;
constexpr AccessFlags InputAttachmentRead           = 0b00001000000000000;
constexpr AccessFlags ShaderRead                    = 0b00000100000000000;
constexpr AccessFlags ShaderWrite                   = 0b00000010000000000;
constexpr AccessFlags ColorAttachmentRead           = 0b00000001000000000;
constexpr AccessFlags ColorAttachmentWrite          = 0b00000000100000000;
constexpr AccessFlags DepthStencilAttachmentRead    = 0b00000000010000000;
constexpr AccessFlags DepthStencilAttachmentWrite   = 0b00000000001000000;
constexpr AccessFlags TransferRead                  = 0b00000000000100000;
constexpr AccessFlags TransferWrite                 = 0b00000000000010000;
constexpr AccessFlags HostRead                      = 0b00000000000001000;
constexpr AccessFlags HostWrite                     = 0b00000000000000100;
constexpr AccessFlags MemoryRead                    = 0b00000000000000010;
constexpr AccessFlags MemoryWrite                   = 0b00000000000000001;
//VK_VERSION_1_3
constexpr AccessFlags None                          = 0b00000000000000000;
}
struct MemoryBarrier {
    AccessFlags srcAccessMask{ AccessFlagBits::None };
    AccessFlags dstAccessMask{ AccessFlagBits::None };
};
struct BufferMemoryBarrier : MemoryBarrier {
    uint32_t    srcQueueFamilyIndex{ 0 };
    uint32_t    dstQueueFamilyIndex{ 0 };
    OCRA::Buffer::Handle buffer;
    uint64_t    offset{ 0 };
    uint64_t    size{ 0 };
};
struct ImageMemoryBarrier : MemoryBarrier {
    uint32_t    srcQueueFamilyIndex{ 0 };
    uint32_t    dstQueueFamilyIndex{ 0 };
    Image::Handle image;
};
void PipelineBarrier(
    const Buffer::Handle& a_CommandBuffer,
    const Pipeline::StageFlags& srcStageMask,
    const Pipeline::StageFlags& dstStageMask,
    const DependencyFlags& dependencyFlags,
    const std::vector<MemoryBarrier>&       a_MemoryBarriers,
    const std::vector<BufferMemoryBarrier>& a_BufferMemoryBarriers,
    const std::vector<ImageMemoryBarrier>&  a_ImageMemoryBarriers);
}