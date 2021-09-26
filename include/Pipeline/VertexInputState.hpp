/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:25:52
*/

#pragma once

#include <Scalar.hpp>

#include <vector>

namespace OCRA::Pipeline::VertexInputState {
struct BindingDescription {
    Uint32 location { 0 }; //location in the shader
    Uint32 binding { 0 }; //binding point
    Uint32 stride { 0 }; //byte stride
    enum class InputRate {
        Vertex, //use vertex attribute
        Instance //use instance index
    } inputRate { InputRate::Vertex }; //source of the data
};
struct AttributeDescription {
    Uint32 location { 0 }; //shaderlocation of this attribute
    Uint32 binding { 0 }; //binding number this attribute takes its data from
    Uint32 offset { 0 }; //byte offset relative to start
};
struct Info {
    std::vector<BindingDescription> vertexBindingDescriptions;
    std::vector<AttributeDescription> vertexAttributeDescriptions;
};
}