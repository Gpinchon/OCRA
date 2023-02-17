#pragma once

#include <OCRA/Handle.hpp>
#include <OCRA/Descriptor/Set.hpp>

#include <GL/Common/Assert.hpp>
#include <GL/Descriptor/Binding.hpp>

#include <memory_resource>

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
        OCRA_ASSERT(a_Write.dstBinding < bindings.size());
        auto& dstBinding = bindings.at(a_Write.dstBinding);
        dstBinding = a_Write;
    }
    void Copy(const CopyOperation& a_Copy) {
        OCRA_ASSERT(a_Copy.dstBinding < bindings.size());
        OCRA_ASSERT(a_Copy.srcBinding < a_Copy.srcSet->bindings.size());
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
