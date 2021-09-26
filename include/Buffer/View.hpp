/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:27:25
*/
#pragma once

#include <Handle.hpp>
#include <Scalar.hpp>

namespace OCRA::Buffer::View {
struct Info {
    Buffer::Handle buffer;
    Uint64 offset { 0 }; //offset in bytes
    Uint64 range { 0 }; //size of the view
}
}