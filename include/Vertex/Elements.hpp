/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:20
*/
#pragma once

#include <Handle.hpp>
#include <Scalar.hpp>

namespace OCRA::Vertex::Elements {
struct Info {
    struct Format { //specifies the organization of an element buffer
        enum class Type {
            Unknown,
            Uint8,
            Uint16,
            Uint32,
            MaxValue
        } type { Type::Unknown };
        Uint64 relativeOffset { 0 };
    } format;
    Uint64 count { 0 };
    Buffer::Handle buffer { 0 };
};
}