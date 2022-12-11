/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-06 19:39:36
*/
#pragma once

#include <OCRA/Common/Extent3D.hpp>
#include <OCRA/Handle.hpp>
#include <OCRA/Image/Format.hpp>
#include <OCRA/Image/Type.hpp>

#include <OCRA/Buffer.hpp>
#include <OCRA/Common/Offset3D.hpp>
#include <OCRA/Common/ColorValue.hpp>
#include <OCRA/Command/Buffer.hpp>
#include <OCRA/Image/Layout.hpp>
#include <OCRA/Image/Subresource.hpp>

#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Image);
OCRA_DECLARE_HANDLE(OCRA::Device);

namespace OCRA::Image {
struct BufferCopy {
    uint64_t bufferOffset{ 0 };
    uint32_t bufferRowLength{ 0 };
    uint32_t bufferImageHeight{ 0 };
    Image::Subresource::Layers imageSubresource;
    Offset3D imageOffset;
    Extent3D imageExtent;
};
struct Copy {
    Image::Subresource::Layers srcSubresource;
    Offset3D                   srcOffset;
    Image::Subresource::Layers dstSubresource;
    Offset3D                   dstOffset;
    Extent3D                   extent{};
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
    Extent3D extent{};
    uint16_t mipLevels { 0 }, arrayLayers { 0 };
    bool fixedSampleLocations { false };
};
Handle Create(const Device::Handle& a_Device, const Info& a_Info);
void CopyBufferToImage(
    const Device::Handle& a_Device,
    const OCRA::Buffer::Handle& a_SrcBuffer,
    const Image::Handle& a_DstImage,
    const std::vector<Image::BufferCopy>& a_Regions);
void CopyImageToBuffer(
    const Device::Handle& a_Device,
    const OCRA::Buffer::Handle& a_DstBuffer,
    const Image::Handle& a_SrcImage,
    const std::vector<Image::BufferCopy>& a_Regions);
void GenerateMipMap(
    const Device::Handle& a_Device,
    const Image::Handle& a_Image);
}

#include <OCRA/Buffer.hpp>
#include <OCRA/Common/Offset3D.hpp>
#include <OCRA/Common/ColorValue.hpp>
#include <OCRA/Command/Buffer.hpp>
#include <OCRA/Image/Layout.hpp>
#include <OCRA/Image/Subresource.hpp>

namespace OCRA::Command
{
void CopyBufferToImage(
    const Command::Buffer::Handle&        a_CommandBuffer,
    const OCRA::Buffer::Handle&           a_SrcBuffer,
    const Image::Handle&                  a_DstImage,
    const std::vector<Image::BufferCopy>& a_Regions);
void CopyImageToBuffer(
    const Command::Buffer::Handle&        a_CommandBuffer,
    const OCRA::Buffer::Handle&           a_DstBuffer,
    const Image::Handle&                  a_SrcImage,
    const std::vector<Image::BufferCopy>& a_Regions);
void CopyImage(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const Image::Handle&            a_SrcImage,
    const Image::Handle&            a_DstImage,
    const std::vector<Image::Copy>  a_Regions);
void GenerateMipMap(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const Image::Handle&            a_Image);
void ClearColorImage(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const Image::Handle&            a_Image,
    const Image::Layout&            a_ImageLayout,
    const ColorValue&               a_Color,
    const std::vector<Image::Subresource::Range>& a_Ranges);
}