/*
 * @Author: gpinchon
 * @Date:   2021-09-26 00:00:00
 * @Last Modified by:   gpinchon
 * @Last Modified time: 2021-10-06 19:39:36
 */
#pragma once

#include <OCRA/Structs.hpp>

#include <vector>

namespace OCRA::Image {
/**
 * @return the byte size of the texture
 */
size_t GetDataSize(const Image::Handle& a_Image);

/**
 * @brief Copies the specified buffer to the specified image.
 * @arg a_SrcBuffer the buffer that will be used for copy
 * @arg a_DstImage  the image the buffer will be copied to
 * @arg a_DstImageLayout the layout the image will be transitioned to after copy
 * @arg a_Regions the regions that will be copied
 */
void CopyBufferToImage(
    const OCRA::Buffer::Handle& a_SrcBuffer,
    const Image::Handle& a_DstImage,
    const ImageLayout& a_DstImageLayout,
    const std::vector<ImageBufferCopy>& a_Regions);

/**
 * @brief Copies the specified image to the specified buffer.
 * No layout transition is done here, meaning you have to do it manually
 * @arg a_DstBuffer the buffer the image will be copied to
 * @arg a_SrcImage  the image that will be used for copy
 * @arg a_SrcImageLayout the current layout of the image
 * @arg a_DstImageLayout the layout the image will be transitioned to after copy
 * @arg a_Regions the regions that will be copied
 */
void CopyImageToBuffer(
    const OCRA::Buffer::Handle& a_DstBuffer,
    const Image::Handle& a_SrcImage,
    const ImageLayout& a_SrcImageLayout,
    const ImageLayout& a_DstImageLayout,
    const std::vector<ImageBufferCopy>& a_Regions);

/**
 * @brief Generates the mipmaps for the specified Image.
 * On OpenGL this is a native function, on VK it's emulated.
 * This command is asynchronous, meaning that you have to synchronize
 * it if you wanna read from the buffer's memory.
 * You can force sync by using WaitIdle.
 */
void GenerateMipMap(const Image::Handle& a_Image);
}
