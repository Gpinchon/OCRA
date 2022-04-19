#pragma once

#include <Handle.hpp>
#include <Image/Format.hpp>
#include <Image/ColorSpace.hpp>
#include <Image/Usage.hpp>
#include <Common/Extent2D.hpp>
#include <Common/SharingMode.hpp>

#include <bitset>
#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::SwapChain);
OCRA_DECLARE_HANDLE(OCRA::Surface);

namespace OCRA::SwapChain
{
using TransformFlags = std::bitset<9>;
namespace TransformFlagsBits {
static TransformFlags None                      = 0b000000000;
static TransformFlags Identity                  = 0b100000000;
static TransformFlags Rotate90                  = 0b010000000;
static TransformFlags Rotate180                 = 0b001000000;
static TransformFlags Rotate270                 = 0b000100000;// VK_VERSION_1_1
static TransformFlags HorizontalMirror          = 0b000010000;// VK_VERSION_1_2
static TransformFlags HorizontalMirrorRotate90  = 0b000001000;// VK_VERSION_1_2
static TransformFlags HorizontalMirrorRotate180 = 0b000000100;// VK_VERSION_1_2
static TransformFlags HorizontalMirrorRotate270 = 0b000000010;// VK_VERSION_1_2
static TransformFlags Inherit                   = 0b000000001;// VK_VERSION_1_2
}
using CompositeAlphaFlags = std::bitset<4>;
namespace CompositeAlphaFlagsBits {
static CompositeAlphaFlags None             = 0b0000;
static CompositeAlphaFlags Opaque           = 0b1000;
static CompositeAlphaFlags PreMultiplied    = 0b0100;
static CompositeAlphaFlags PostMultiplied   = 0b0010;
static CompositeAlphaFlags Inherit          = 0b0001;
}
enum PresentMode {
    Immediate,
    Mailbox,
    Fifo,
    FifoRelaxed,
    MaxValue
};
struct Info {
    Surface::Handle         surface;
    uint32_t                minImageCount;
    Image::Format           imageFormat;
    Image::ColorSpace       imageColorSpace;
    Extent2D                imageExtent;
    uint32_t                imageArrayLayers;
    Image::UsageFlags       imageUsage;
    SharingMode             imageSharingMode;
    std::vector<uint32_t>   queueFamilyIndices;
    TransformFlags          preTransform;
    CompositeAlphaFlags     compositeAlpha;
    PresentMode             presentMode;
    bool                    clipped;
    Handle                  oldSwapchain;
};
Handle Create(const Device::Handle& a_Device, const Info& a_Info);
}