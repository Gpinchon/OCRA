/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:17
*/
#pragma once

#include <Handle.hpp>

namespace OCRA::Vertex::Buffer {
unsigned GetGLHandle(const Device::Handle& a_Device, const Handle& a_Handle);
}