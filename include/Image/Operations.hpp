/*
* @Author: gpinchon
* @Date:   2021-10-06 19:37:06
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-06 19:43:12
*/
#pragma once

#include <Extent3D.hpp>
#include <Handle.hpp>
#include <Image/Image.hpp>
#include <Offset3D.hpp>
#include <Scalar.hpp>

#include <vector>

namespace OCRA::Image {
struct SubresourceLayers {
    /*
    * //TODO support aspect copy for OGL
    enum class Aspect {
        Unknown = -1, Color, Depth, Stencil, DepthStencil, MaxValue
    } aspect{ Aspect::Unknown };
    */
    Uint32 level{ 0 }; //indicates the base level (mipmap or array layer) used for the copy
};
struct BufferCopy {
    Uint64 bufferOffset{ 0 };
    Uint32 bufferRowLength{ 0 };
    Uint32 bufferImageHeight{ 0 };
    SubresourceLayers imageSubresource;
    Offset3D imageOffset;
    Extent3D imageExtent;
};
void CopyBufferToImage(
    const Device::Handle& a_Device,
    const Buffer::Transfer::Handle& a_srcBuffer,
    const Image::Handle& a_dstImage,
    const std::vector<BufferCopy>& a_Regions);
void CopyImageToBuffer(
    const Device::Handle& a_Device,
    const Buffer::Transfer::Handle& a_DstBuffer,
    const Image::Handle& a_SrcImage,
    const std::vector<BufferCopy>& a_Regions);
}