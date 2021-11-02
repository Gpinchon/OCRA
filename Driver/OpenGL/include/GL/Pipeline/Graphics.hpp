/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 15:03:53
*/
#include <Handle.hpp>

#include <functional>

namespace OCRA::Pipeline::Graphics
{
void ApplyGraphicsStates(const Device::Handle& a_Device, const Handle& a_GraphicsPipeline);
std::function<void()> Compile(const Device::Handle& a_Device, const Handle& a_GraphicsPipeline);
}