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
OCRA_DECLARE_HANDLE(OCRA::Queue);
OCRA_DECLARE_HANDLE(OCRA::Queue::Semaphore);
OCRA_DECLARE_HANDLE(OCRA::Image);

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
    Immediate = 0,
    Fifo = 1,
    FifoRelaxed = -1,
    MaxValue
};
struct Info {
    Surface::Handle         surface{ nullptr };
    uint32_t                minImageCount{ 0 };
    Image::Format           imageFormat{ Image::Format::Unknown };
    Image::ColorSpace       imageColorSpace{ Image::ColorSpace::Unknown };
    Extent2D                imageExtent{ 0, 0 };
    uint32_t                imageArrayLayers{ 0 };
    Image::UsageFlags       imageUsage{ Image::UsageFlagBits::None };
    SharingMode             imageSharingMode{ SharingMode::Exclusive };
    std::vector<uint32_t>   queueFamilyIndices;
    TransformFlags          preTransform{ TransformFlagsBits::None };
    CompositeAlphaFlags     compositeAlpha{ CompositeAlphaFlagsBits::None };
    PresentMode             presentMode{ PresentMode::Immediate };
    bool                    clipped{ false };
    Handle                  oldSwapchain{ nullptr };
};
Handle Create(const Device::Handle& a_Device, const Info& a_Info);
struct PresentInfo {
    std::vector<Queue::Semaphore::Handle>   waitSemaphores;
    std::vector<SwapChain::Handle>          swapChains;
    std::vector<uint32_t>                   imageIndices;
};
const std::vector<Image::Handle>& GetImages(const Device::Handle& a_Device, const Handle& a_SwapChain);
void Present(const Queue::Handle& a_Queue, const PresentInfo& a_PresentInfo);
}