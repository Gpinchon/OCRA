/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:28:36
*/
#pragma once

#include <Common/DepthBounds.hpp>
#include <Compare.hpp>
#include <Stencil.hpp>

namespace OCRA::Pipeline::DepthStencilState {
struct Info {
    bool depthTestEnable { true };
    bool depthWriteEnable { true };
    Compare::Operation depthCompareOp { Compare::Operation::Less };
    bool depthBoundsTestEnable { false };
    bool stencilTestEnable { false };
    Stencil::OpState frontStencilOpState;
    Stencil::OpState backStencilOpState;
    DepthBounds depthBounds{ 0, 1 };
};
}