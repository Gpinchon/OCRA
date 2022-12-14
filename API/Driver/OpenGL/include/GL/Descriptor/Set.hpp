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
        bindings.push_back({ std::forward<Args>(a_Args)... });
    }
    void Write(const WriteOperation& a_Write) {
#ifdef _DEBUG
        assert(a_Write.dstBinding < bindings.size());
#endif
        auto& dstBinding = bindings.at(a_Write.dstBinding);
        dstBinding = a_Write;
    }
    void Copy(const CopyOperation& a_Copy) {
#ifdef _DEBUG
        assert(a_Copy.dstBinding < bindings.size());
        assert(a_Copy.srcBinding < a_Copy.srcSet->bindings.size());
#endif
        auto& dstBinding = bindings.at(a_Copy.dstBinding);
        dstBinding = a_Copy.srcSet->bindings.at(a_Copy.srcBinding);
    }
    void Bind() {
        for (auto& binding : bindings)
            binding.Bind();
    }
    void Unbind() {
        for (auto& binding : bindings)
            binding.Unbind();
    }
    SetLayout::Handle       layout;
    std::vector<Binding>    bindings;
};
}