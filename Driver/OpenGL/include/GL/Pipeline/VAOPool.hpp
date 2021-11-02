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
#include <GL/VertexType.hpp>
#include <GL/glew.h>

#include <stdexcept>

namespace OCRA::Pipeline::VertexInputState {
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
                attribute.location);
            glVertexArrayAttribFormat(
                handle,
                attribute.location,
                attribute.format.size,
                GetGLVertexType(attribute.format.type),
                attribute.format.normalized,
                attribute.offset);
            glVertexArrayAttribBinding(
                handle,
                attribute.location,
                attribute.binding);
        }
    }
    Info info;
    GLuint handle { 0 };
};
static inline bool operator!=(const AttributeDescription::Format& a_lAttribFormat, const AttributeDescription::Format& a_rAttribFormat) noexcept
{
    return a_lAttribFormat.type != a_rAttribFormat.type
        || a_lAttribFormat.size != a_rAttribFormat.size
        || a_lAttribFormat.normalized != a_rAttribFormat.normalized;
}
static inline bool operator!=(const AttributeDescription& a_lAttrib, const AttributeDescription& a_rAttrib) noexcept
{
    return a_lAttrib.offset != a_rAttrib.offset
        || a_lAttrib.binding != a_rAttrib.binding
        || a_lAttrib.location != a_rAttrib.location
        || a_lAttrib.format != a_rAttrib.format;
}
static inline bool operator!=(const BindingDescription& a_lBinding, const BindingDescription& a_rBinding) noexcept
{
    return a_lBinding.binding != a_rBinding.binding
        || a_lBinding.stride != a_rBinding.stride
        || a_lBinding.inputRate != a_rBinding.inputRate;
}
static inline bool operator!=(const Info& a_lInfo, const Info& a_rInfo) noexcept
{
    return a_lInfo.primitiveRestartIndex != a_rInfo.primitiveRestartIndex
        || a_lInfo.attributeDescriptionCount != a_rInfo.attributeDescriptionCount
        || a_lInfo.bindingDescriptionCount != a_rInfo.bindingDescriptionCount
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
        ObjectPoolType::ReleaseObject(a_Index);
    }
};
}