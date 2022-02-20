/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-06 20:06:29
*/
#pragma once

#include <Handle.hpp>
#include <Command/Image.hpp>

#include <vector>

namespace OCRA::Image {
unsigned GetGLHandle(const Device::Handle& a_Device, const Handle& a_Handle);
void CopyBufferToImage(
    const Command::Buffer::Handle& a_CommandBuffer,
    const OCRA::Buffer::Transfer::Handle& a_srcBuffer,
    const Image::Handle& a_dstImage,
    const std::vector<Command::BufferImageCopy>& a_Regions);
void CopyImageToBuffer(
    const Command::Buffer::Handle& a_CommandBuffer,
    const OCRA::Buffer::Transfer::Handle& a_DstBuffer,
    const Image::Handle& a_SrcImage,
    const std::vector<Command::BufferImageCopy>& a_Regions);
}