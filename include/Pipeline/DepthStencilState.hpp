/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:28:36
*/

#include <Compare.hpp>
#include <Stencil.hpp>

namespace OCRA::Pipeline::DepthStencilState {
struct StencilOpState {
    Stencil::Operation failOp { Stencil::Operation::Keep }; //the operation to be realized when stencil test FAILS
    Stencil::Operation passOp { Stencil::Operation::Keep }; //the operation to be realized when stencil test PASSES
    Stencil::Operation depthFailOp { Stencil::Operation::Keep }; //the operation to be realized when stencil test PASSES but depth test FAILS
    Compare::Operation compareOp{ Compare::Operation ::Always };
    Uint32 compareMask { 1 }; //a mask that is ANDed with ref and the buffer's content
    Uint32 writeMask { 1 }; //a mask that is ANDed with the stencil value about to be written to the buffer
    Uint32 reference { 0 }; //the reference value used in comparison.
};
struct Info {
    bool depthTestEnable { true };
    bool depthWriteEnable { true };
    Compare::Operation depthCompareOp { Compare::Operation::Less };
    bool depthBoundsTestEnable { false };
    bool stencilTestEnable { false };
    StencilOpState front;
    StencilOpState back;
    float minDepthBounds { 0 };
    float maxDepthBounds { 1 };
};
}