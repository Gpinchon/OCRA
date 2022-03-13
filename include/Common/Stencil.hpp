/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:27:18
*/
#pragma once

#include <Common/Compare.hpp>

namespace OCRA::Stencil {
enum class Operation {
    Keep = 0,
    Zero = 1,
    Replace = 2,
    IncrementAndClamp = 3,
    DecrementAndClamp = 4,
    Invert = 5,
    IncrementAndWrap = 6,
    DecrementAndWrap = 7,
    MaxValue
};
struct OpState {
    Operation failOp { Operation::Keep }; //the operation to be realized when stencil test FAILS
    Operation passOp { Operation::Keep }; //the operation to be realized when stencil test PASSES
    Operation depthFailOp { Operation::Keep }; //the operation to be realized when stencil test PASSES but depth test FAILS
    Compare::Operation compareOp{ Compare::Operation ::Always };
    uint32_t compareMask { 1 }; //a mask that is ANDed with ref and the buffer's content
    uint32_t writeMask { 1 }; //a mask that is ANDed with the stencil value about to be written to the buffer
    uint32_t reference { 0 }; //the reference value used in comparison.
};
}
