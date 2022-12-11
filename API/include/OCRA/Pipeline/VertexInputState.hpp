/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:25:52
*/

#pragma once

#include <OCRA/Handle.hpp>
#include <OCRA/Common/IndexType.hpp>
#include <OCRA/Common/VertexType.hpp>

#include <vector>

namespace OCRA::Pipeline::VertexInputState {
struct AttributeDescription {
    struct Format {
        uint8_t size{ 0 }; //Number of components per vertex
        VertexType type{ VertexType::None }; //Type of data of each components
        bool normalized{ false };
    } format;
    uint32_t offset{ 0 }; //(Relative offset) the distance between elements in the buffer
    uint8_t binding{ 0 }; //The binding number this attribute takes its data from
    uint32_t location{ 0 }; //Location in the shader for this attribute
};
struct BindingDescription {
    uint32_t binding{ 0 }; //index inside the BindVertexBuffers Command
    uint32_t stride{ 0 }; //byte stride
    enum class InputRate {
        Vertex, //use vertex attribute
        Instance //use instance index
    } inputRate{ InputRate::Vertex }; //is the data divided by vertex or by instance ?
};
struct Info {
    uint32_t primitiveRestartIndex{ 0 };
    std::vector<AttributeDescription> attributeDescriptions;
    std::vector<BindingDescription> bindingDescriptions;
};
}