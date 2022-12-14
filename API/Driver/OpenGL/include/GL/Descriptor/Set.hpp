#pragma once

#include <OCRA/Handle.hpp>
#include <OCRA/Descriptor/Set.hpp>

#include <GL/Descriptor/SetData.hpp>

#include <vector>

#ifdef _DEBUG
#include <cassert>
#endif

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::SetLayout);

namespace OCRA::Descriptor::Set
{
struct Impl
{
    Impl() = default;
    Impl(const SetLayout::Handle& a_Layout)
        : layout(a_Layout)
    {
        for (const auto& binding : a_Layout->bindings) {
            PushData(binding.type, binding.binding);
        }
    }
    template<typename... Args>
    void PushData(Args&&... a_Args) {
        data.push_back({ std::forward<Args>(a_Args)... });
    }
    void Write(const WriteOperation& a_Write) {
#ifdef _DEBUG
        assert(a_Write.dstBinding < data.size());
#endif
        auto& dstData = data.at(a_Write.dstBinding);
        dstData = a_Write;
    }
    void Copy(const CopyOperation& a_Copy) {
#ifdef _DEBUG
        assert(a_Copy.dstBinding < data.size());
        assert(a_Copy.srcBinding < a_Copy.srcSet->data.size());
#endif
        auto& dstData = data.at(a_Copy.dstBinding);
        const auto& srcData = a_Copy.srcSet->data.at(a_Copy.srcBinding);
        dstData = srcData;
    }
    void Bind() {
        for (auto& data : data)
            data.Bind();
    }
    void Unbind() {
        for (auto& data : data)
            data.Unbind();
    }
    SetLayout::Handle       layout;
    std::vector<Data>       data;
};
}