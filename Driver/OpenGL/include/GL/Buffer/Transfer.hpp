/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 15:03:29
*/
#pragma once

#include <Handle.hpp>

#include <GL/Handle.hpp>

namespace OCRA::Buffer::Transfer
{
Buffer::Handle GetBufferHandle(const Device::Handle& a_Device, const Handle& a_Handle);
}