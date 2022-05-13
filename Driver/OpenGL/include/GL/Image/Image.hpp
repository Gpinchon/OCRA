/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-06 20:06:29
*/
#pragma once

#include <Handle.hpp>
#include <Image/Image.hpp>

#include <GL/WeakHandle.hpp>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_WEAK_HANDLE(OCRA::Device)

namespace OCRA::Image {
struct Impl
{
    Impl(const Device::Handle& a_Device, const Info& a_Info, const uint32_t a_Target);
    ~Impl();
    virtual void Download(const Command::BufferImageCopy& a_Copy, const size_t& a_MemoryOffset) = 0;
    virtual void Upload(const Command::BufferImageCopy& a_Copy, const size_t& a_MemoryOffset) = 0;
    const Device::WeakHandle device;
    const Info info;
    const uint32_t internalFormat;
    const uint32_t dataType;
    const uint32_t dataFormat;
    const uint32_t handle;
    const uint32_t target;
};
/**
* @brief creates an empty image with no storage
*/
Handle CreateEmpty(const Device::Handle& a_Device, const Info& a_Info);
}