/*
 * @Author: gpinchon
 * @Date:   2021-09-26 00:00:00
 * @Last Modified by:   gpinchon
 * @Last Modified time: 2021-10-06 20:06:29
 */
#pragma once

#include <OCRA/Core.hpp>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Image {
struct Impl {
    Impl(const Device::Handle& a_Device, const CreateImageInfo& a_Info, const uint32_t a_Target);
    ~Impl();
    virtual void Download(const ImageBufferCopy& a_Copy, const size_t& a_MemoryOffset) = 0;
    virtual void Upload(const ImageBufferCopy& a_Copy, const size_t& a_MemoryOffset)   = 0;
    const Device::WeakHandle device;
    const CreateImageInfo info;
    const uint32_t internalFormat;
    const uint32_t dataType;
    const uint32_t dataFormat;
    const uint32_t target;
    uint32_t handle;
};
/**
 * @brief checks if the specified copy operation is valid for this image
 */
void CheckValidCopy(const ImageBufferCopy& a_Copy, const Image::Handle& a_Image);
}
