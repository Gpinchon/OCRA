/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:27:05
*/
#pragma once

#include <Handle.hpp>

namespace OCRA::Queue {
void PushPass(const Device::Handle& a_Device, const Pass::Handle& a_Pass);
void FlushPass(const Device::Handle& a_Device, const Pass::Handle& a_Pass);
}