#pragma once

#include <Handle.hpp>
#include <Descriptor/Set.hpp>

#include <GL/Descriptor/SetData.hpp>

#include <vector>
#include <cassert>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::SetLayout);

namespace OCRA::Descriptor::Set
{
struct Impl
{
    Impl(const SetLayout::Handle& a_Layout)
        : layout(a_Layout)
    {
        data.reserve(a_Layout->bindings.size());
        for (const auto& binding : a_Layout->bindings) {
            data.push_back(Data(binding.type, binding.binding));
        }
    };
    void Write(const WriteOperation& a_Write) {
        auto& dstData = data.at(a_Write.dstBinding);
        dstData = a_Write;
    }
    void Copy(const CopyOperation& a_Copy) {
        auto& dstData = data.at(a_Copy.dstBinding);
        const auto& srcData = a_Copy.srcSet->data.at(a_Copy.dstBinding);
        dstData = srcData;
    }
    void Bind() {
        for (auto& setData : data)
            setData.Bind();
    }
    void Unbind() {
        for (auto& setData : data)
            setData.Unbind();
    }
    const SetLayout::Handle layout;
    std::vector<Data> data;
};
}