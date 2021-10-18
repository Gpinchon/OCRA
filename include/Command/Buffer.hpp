/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/
#pragma once

#include <Handle.hpp>

namespace OCRA::Command::Buffer
{
Handle Create(const Device::Handle& a_Device);
void Destroy(const Device::Handle& a_Device, const Handle& a_CommandBuffer);
//Begin Command Buffer recording
void Begin(const Handle& a_CommandBuffer);
//End Command Buffer recording
void End(const Handle& a_CommandBuffer);
}