#pragma once

#include <OCRA/Handle.hpp>
#include <OCRA/Descriptor/Set.hpp>

#include <GL/Descriptor/Binding.hpp>

#include <memory_resource>

#ifdef _DEBUG
#include <cassert>
#endif

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::SetLayout);

namespace OCRA::Descriptor::Set
{
struct Impl
{
    Impl(std::pmr::memory_resource* a_MemoryResource)
        : bindings(a_MemoryResource)
    {}
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
    std::pmr::vector<Binding> bindings;
};
}
