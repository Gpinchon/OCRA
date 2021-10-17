/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/
#pragma once

#include <Extent3D.hpp>
#include <Handle.hpp>
#include <Image/Image.hpp>
#include <Offset3D.hpp>
#include <Scalar.hpp>

namespace OCRA::Command
{
struct SubresourceLayers {
    /*
    * //TODO support aspect copy for OGL
    enum class Aspect {
        Unknown = -1, Color, Depth, Stencil, DepthStencil, MaxValue
    } aspect{ Aspect::Unknown };
    */
    Uint32 level{ 0 }; //indicates the base level (mipmap or array layer) used for the copy
};
struct BufferImageCopy {
    Uint64 bufferOffset{ 0 };
    Uint32 bufferRowLength{ 0 };
    Uint32 bufferImageHeight{ 0 };
    SubresourceLayers imageSubresource;
    Offset3D imageOffset;
    Extent3D imageExtent;
};
void CopyBufferToImage(
    const CommandBuffer::Handle& a_CommandBuffer,
    const Buffer::Transfer::Handle& a_srcBuffer,
    const Image::Handle& a_dstImage,
    const std::vector<BufferImageCopy>& a_Regions);
void CopyImageToBuffer(
    const CommandBuffer::Handle& a_CommandBuffer,
    const Buffer::Transfer::Handle& a_DstBuffer,
    const Image::Handle& a_SrcImage,
    const std::vector<BufferImageCopy>& a_Regions);
}