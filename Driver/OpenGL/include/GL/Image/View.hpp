/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:31:47
*/
#pragma once

#include <Handle.hpp>

namespace OCRA::Image::View {
unsigned GetGLHandle(const Device::Handle& a_Device, const Handle& a_Handle);
}