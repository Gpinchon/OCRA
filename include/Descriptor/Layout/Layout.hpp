/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:27:32
*/
#pragma once

#include <Handle.hpp>

namespace OCRA::Layout {
struct Info { //describes a list of bindings
    constexpr auto MaxBindings = 32;
    uint8_t bindingCount { 0 };
    Binding::Handle bindings[MaxBindings];
}
}