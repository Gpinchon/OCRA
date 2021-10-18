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
#include <VertexType.hpp>

#include <array>

namespace OCRA::Pipeline::VertexInputState {
struct AttributeDescription {
    struct Format {
		Uint8 size{ 0 }; //Number of components per vertex
		VertexType type{ VertexType::None }; //Type of data of each components
		bool normalized{ false };
	} format;
    Uint32 offset{ 0 };
	Uint8 binding{ 0 }; //BindingDescription index
    Uint32 location{ 0 }; //Location in the shader for this attribute
};
struct BindingDescription {
	Uint32 binding{ 0 }; //index inside the BindVertexBuffers Command
    Uint32 stride{ 0 }; //byte stride
    enum class InputRate {
        Vertex, //use vertex attribute
        Instance //use instance index
    } inputRate{ InputRate::Vertex }; //source of the data
};
struct Info {
    static constexpr auto MaxAttributes = 32;
    static constexpr auto MaxBindings = 32;
    Uint32 primitiveRestartIndex{ 0 };
    Uint8 attributeDescriptionCount{ 0 };
    std::array<AttributeDescription, MaxAttributes> attributeDescriptions;
    Uint8 bindingDescriptionCount{ 0 };
    std::array<BindingDescription, MaxBindings> bindingDescriptions;
};
}