/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:17
*/
#pragma once

#include <Handle.hpp>
#include <Scalar.hpp>

namespace OCRA::Vertex::Array {
struct Attribute {
    struct Format { //specifies the organization of an array's attribute
        enum class Type {
            Unknown = -1,
            Float32,
            Float16,
            Int32,
            Uint32,
            Int16,
            Uint16,
            Int8,
            Uint8,
            MaxValue
        } type { Type::Unknown };
        Uint64 relativeOffset { 0 }; //distance between elements in buffer
        Uint8 valuesPerVertex { 0 }; //number of values per vertex
        bool normalized { false };
    } format;
    Int32 relativeOffset { 0 }; //starting index in the array
    Uint64 count { 0 }; //number of indices to be rendered
    Buffer::Handle buffer { 0 };
};
struct Info {
    constexpr auto MaxAttributes = 32;
    bool primitiveRestart { false };
    Uint64 primitiveRestartIndex { 0 };
    Uint16 attributeCount { 0 };
    Attribute attributes[MaxAttributes];
};
}