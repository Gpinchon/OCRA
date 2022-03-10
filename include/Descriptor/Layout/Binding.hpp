/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:27:30
*/
#pragma once

#include <Shader/Stage.hpp>

namespace OCRA::Layout::Binding {
struct Info { //describes a binding in shader
    enum class Type {
        SampledImage,
        StorageImage,
        UniformTexelBuffer,
        StorageTexelBuffer,
        UniformBuffer,
        StorageBuffer,
        UniformBufferDynamic,
        StorageBufferDynamic,
        InputAttachment,
        InlineUniformBlock,
        AccelerationStructure, //for RayTracing
        MutableValve
    } descriptorType;
    uint8_t index { 0 }; //binding point in the shader
    uint64_t descriptorCount { 1 }; //number of values in array
    Shader::Stage::Flags stageFlags { Shader::Stage::Flags::None };
}
}