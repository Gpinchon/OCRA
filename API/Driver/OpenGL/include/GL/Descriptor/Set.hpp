#pragma once

#include <OCRA/Handle.hpp>
#include <OCRA/Descriptor/Set.hpp>

#include <GL/Descriptor/Binding.hpp>

#include <array>

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
    void Write(const WriteOperation& a_Write) {
#ifdef _DEBUG
        assert(a_Write.dstBinding < bindingCount);
#endif
        auto& dstBinding = bindings.at(a_Write.dstBinding);
        *dstBinding = a_Write;
    }
    void Copy(const CopyOperation& a_Copy) {
#ifdef _DEBUG
        assert(a_Copy.dstBinding < bindingCount);
        assert(a_Copy.srcBinding < a_Copy.srcSet->bindingCount);
#endif
        auto& dstBinding = bindings.at(a_Copy.dstBinding);
        dstBinding = a_Copy.srcSet->bindings.at(a_Copy.srcBinding);
    }
    void Bind() {
        for (uint16_t bindingIndex = 0; bindingIndex < bindingCount; bindingIndex++)
            bindings.at(bindingIndex)->Bind();
    }
    void Unbind() {
        for (uint16_t bindingIndex = 0; bindingIndex < bindingCount; bindingIndex++)
            bindings.at(bindingIndex)->Unbind();
    }
    uint16_t bindingCount = 0;
    std::array<std::shared_ptr<Binding>, 256> bindings;
};
}