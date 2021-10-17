/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:25:52
*/

#pragma once

#include <Handle.hpp>
#include <Scalar.hpp>
#include <IndexType.hpp>

#include <array>

namespace OCRA::Pipeline::VertexInputState {
struct AttributeDescription {
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
        } type{ Type::Unknown };
        Uint32 relativeOffset{ 0 }; //distance between elements in buffer
        Uint8 valuesPerVertex{ 0 }; //number of values per vertex
        bool normalized{ false };
    } format;
    Uint8 bindingIndex{ 0 }; //BindingDescription index
};
struct BindingDescription {
    Buffer::Vertex::Handle buffer{ 0 };
    Int32 offset{ 0 }; //starting index in the array
    Uint32 stride{ 0 }; //byte stride
    enum class InputRate {
        Vertex, //use vertex attribute
        Instance //use instance index
    } inputRate{ InputRate::Vertex }; //source of the data
};
struct IndexBufferDescription {
    struct Format { //specifies the organization of an element buffer
        IndexType type{ IndexType::None };
    } format;
    Buffer::Vertex::Handle buffer{ 0 };
};
struct Info {
    static constexpr auto MaxAttributes = 32;
    static constexpr auto MaxBindings = 32;
    Uint32 primitiveRestartIndex{ 0 };
    Uint8 attributeDescriptionCount{ 0 };
    std::array<AttributeDescription, MaxAttributes> attributeDescriptions;
    Uint8 bindingDescriptionCount{ 0 };
    std::array<BindingDescription, MaxBindings> bindingDescriptions;
	IndexBufferDescription indexBufferDescription;
};
}