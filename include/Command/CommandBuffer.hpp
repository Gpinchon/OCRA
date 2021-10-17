/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/
#pragma once

#include <Handle.hpp>

namespace OCRA::CommandBuffer
{
Handle Create(const Device::Handle& a_Device);
void Destroy(const Handle& a_CommandBuffer);
void Flush(const Handle& a_CommandBuffer);
}