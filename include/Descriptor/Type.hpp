#pragma once

namespace OCRA::Descriptor
{
enum class Type {
    Unknown = -1,
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
    MutableValve,
    MaxValue
};
}