/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:25:22
*/
#pragma once

#include <Primitive/Topology.hpp>

namespace OCRA::Pipeline::InputAssemblyState {
struct Info {
    Primitive::Topology topology { Primitive::Topology::Unknown };
    bool primitiveRestartEnable { false };
};
}