/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/
#pragma once

#include <Handle.hpp>

namespace OCRA::Device {
struct Info {
    Context::Handle context;
};
void RenderPass(const Device::Handle& a_Device, const RenderPass::Handle& a_Pass);
}