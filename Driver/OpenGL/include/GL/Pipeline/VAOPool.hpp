/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-03 22:11:23
*/
#pragma once

#include <Handle.hpp>
#include <Pipeline/VertexInputState.hpp>

#include <GL/Buffer/Buffer.hpp>
#include <GL/Buffer/Vertex.hpp>
#include <GL/ObjectPool.hpp>
#include <GL/glew.h>

#include <stdexcept>

namespace OCRA::Pipeline::VertexInputState {
static inline GLenum GetGLFormatType(const AttributeDescription::Format::Type& a_Type)
{
    switch (a_Type) {
    case AttributeDescription::Format::Type::Float32:
        return GL_FLOAT;
    case AttributeDescription::Format::Type::Float16:
        return GL_HALF_FLOAT;
    case AttributeDescription::Format::Type::Int32:
        return GL_INT;
    case AttributeDescription::Format::Type::Uint32:
        return GL_UNSIGNED_INT;
    case AttributeDescription::Format::Type::Int16:
        return GL_SHORT;
    case AttributeDescription::Format::Type::Uint16:
        return GL_UNSIGNED_SHORT;
    case AttributeDescription::Format::Type::Int8:
        return GL_BYTE;
    case AttributeDescription::Format::Type::Uint8:
        return GL_UNSIGNED_BYTE;
    default:
        throw std::runtime_error("Unknown Attribute Format Type");
    }
}
struct VAO {
    VAO()
    {
        glCreateVertexArrays(1, &handle);
    }
    ~VAO()
    {
        glDeleteVertexArrays(1, &handle);
    }
    inline void Reset() noexcept
    {
        info = {};
        for (auto attribIndex = 0u; attribIndex < info.MaxAttributes; ++attribIndex)
            glDisableVertexArrayAttrib(
                handle,
                attribIndex);
    }
    inline void Set(const Device::Handle& a_Device, const Info& a_Info)
    {
        info = info;
        for (auto attribIndex = 0u; attribIndex < info.attributeDescriptionCount; ++attribIndex) {
            const auto& attribute { info.attributeDescriptions.at(attribIndex) };
            glEnableVertexArrayAttrib(
                handle,
                attribIndex);
            glVertexArrayAttribFormat(
                handle,
                attribIndex,
                attribute.format.valuesPerVertex,
                GetGLFormatType(attribute.format.type),
                attribute.format.normalized,
                attribute.format.relativeOffset);
            const auto& binding { info.bindingDescriptions.at(attribute.bindingIndex) };
            glVertexArrayAttribBinding(
                handle,
                attribIndex,
                attribute.bindingIndex);
			auto bufferHandle{ Buffer::Vertex::GetBuffeHandle(a_Device, binding.buffer) };
			auto bufferGLHandle{ Buffer::GetGLHandle(a_Device, bufferHandle) };
            glVertexArrayVertexBuffer(
                handle,
                attribute.bindingIndex,
                bufferGLHandle,
                binding.offset,
                binding.stride);
        }
    }
    Info info;
    GLuint handle { 0 };
};
static inline bool operator!=(const AttributeDescription::Format& a_lAttribFormat, const AttributeDescription::Format& a_rAttribFormat) noexcept
{
    return a_lAttribFormat.type != a_rAttribFormat.type
        || a_lAttribFormat.relativeOffset != a_rAttribFormat.relativeOffset
        || a_lAttribFormat.valuesPerVertex != a_rAttribFormat.valuesPerVertex
        || a_lAttribFormat.normalized != a_rAttribFormat.normalized;
}
static inline bool operator!=(const AttributeDescription& a_lAttrib, const AttributeDescription& a_rAttrib) noexcept
{
    return a_lAttrib.bindingIndex != a_rAttrib.bindingIndex
        || a_lAttrib.format != a_rAttrib.format;
}
static inline bool operator!=(const BindingDescription& a_lBinding, const BindingDescription& a_rBinding) noexcept
{
    return a_lBinding.buffer != a_rBinding.buffer
        || a_lBinding.offset != a_rBinding.offset
        || a_lBinding.stride != a_rBinding.stride
        || a_lBinding.inputRate != a_rBinding.inputRate;
}
static inline bool operator!=(const IndexBufferDescription::Format& a_lFormat, const IndexBufferDescription::Format& a_rFormat) noexcept
{
    return a_lFormat.type != a_rFormat.type;
}
static inline bool operator!=(const IndexBufferDescription& a_lBinding, const IndexBufferDescription& a_rBinding) noexcept
{
    return a_lBinding.buffer != a_rBinding.buffer
        || a_lBinding.format != a_rBinding.format;
}
static inline bool operator!=(const Info& a_lInfo, const Info& a_rInfo) noexcept
{
    return a_lInfo.primitiveRestartIndex != a_rInfo.primitiveRestartIndex
        || a_lInfo.attributeDescriptionCount != a_rInfo.attributeDescriptionCount
        || a_lInfo.bindingDescriptionCount != a_rInfo.bindingDescriptionCount
        || a_lInfo.indexBufferDescription != a_rInfo.indexBufferDescription
        || [a_lInfo, a_rInfo]() {
               for (auto attribIndex = 0u; attribIndex < a_lInfo.attributeDescriptionCount; ++attribIndex) {
                   if (a_lInfo.attributeDescriptions.at(attribIndex) != a_rInfo.attributeDescriptions.at(attribIndex))
                       return true;
               }
               return false;
           }()
        || [a_lInfo, a_rInfo]() {
               for (auto bindingIndex = 0u; bindingIndex < a_lInfo.bindingDescriptionCount; ++bindingIndex) {
                   if (a_lInfo.bindingDescriptions.at(bindingIndex) != a_rInfo.bindingDescriptions.at(bindingIndex))
                       return true;
               }
               return false;
           }();
}
static inline bool operator==(const Info& a_lInfo, const Info& a_rInfo) noexcept
{
    return !(a_lInfo != a_rInfo);
}
struct VAOPool : public ObjectPool<VAO>
{
    inline Reference FindSimilar(const Info& a_Info) noexcept
    {
        for (auto& vaoIndex : usedIndices) {
            if (Get(vaoIndex).info == a_Info)
                return Reference(*this, vaoIndex);
        }
        return Reference(*this, -1);
    }
    inline virtual void ReleaseObject(IndexType a_Index) override {
        Get(a_Index).Reset();
        ObjectPool<VAO>::ReleaseObject(a_Index);
    }
};
}