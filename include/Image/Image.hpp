/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-06 19:39:36
*/
#pragma once

#include <Common/Extent3D.hpp>
#include <Handle.hpp>
#include <Image/Format.hpp>

#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Image);
OCRA_DECLARE_HANDLE(OCRA::Device);

namespace OCRA::Image {
enum class Type {
    Unknown = -1,
    Image1D,
    Image2D,
    Image3D,
    MaxValue
};
enum class Samples {
    Samples1 = 1,
    Samples2 = 2,
    Samples4 = 4,
    Samples8 = 8,
    Samples16 = 16,
    Samples32 = 32,
    Samples64 = 64,
    MaxValue
};
//Image Info specs
struct Info {
    Type type { Type::Unknown };
    Samples samples { Samples::Samples1 };
    Image::Format format { Image::Format::Unknown };
    Extent3D extent;
    uint16_t mipLevels { 0 }, arrayLayers { 0 };
    bool fixedSampleLocations { false };
};
Handle Create(const Device::Handle& a_Device, const Info& a_Info);
}

#include <Buffer.hpp>
#include <Common/Offset3D.hpp>
#include <Common/ColorValue.hpp>
#include <Command/Buffer.hpp>
#include <Image/Layout.hpp>

namespace OCRA::Image
{
namespace Subresource
{
struct Layers {
    /*
    * //TODO support aspect copy for OGL
    enum class Aspect {
        Unknown = -1, Color, Depth, Stencil, DepthStencil, MaxValue
    } aspect{ Aspect::Unknown };
    */
    uint32_t level{ 0 }; //indicates the base level (mipmap or array layer) used for the copy
};
struct Range {
    uint32_t              baseMipLevel{ 0 };
    uint32_t              levelCount{ 0 };
    uint32_t              baseArrayLayer{ 0 };
    uint32_t              layerCount{ 0 };
};
}
}

namespace OCRA::Command
{
struct BufferImageCopy {
    uint64_t bufferOffset{ 0 };
    uint32_t bufferRowLength{ 0 };
    uint32_t bufferImageHeight{ 0 };
    Image::Subresource::Layers imageSubresource;
    Offset3D imageOffset;
    Extent3D imageExtent;
};
void CopyBufferToImage(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const OCRA::Buffer::Handle&     a_SrcBuffer,
    const Image::Handle&            a_DstImage,
    const std::vector<BufferImageCopy>& a_Regions);
void CopyImageToBuffer(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const OCRA::Buffer::Handle&     a_DstBuffer,
    const Image::Handle&            a_SrcImage,
    const std::vector<BufferImageCopy>& a_Regions);
void ClearColorImage(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const Image::Handle&            a_Image,
    const Image::Layout&            a_ImageLayout,
    const ColorValue&               a_Color,
    const std::vector<Image::Subresource::Range>& a_Ranges);
}