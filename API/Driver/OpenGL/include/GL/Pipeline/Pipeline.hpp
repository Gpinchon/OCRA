#pragma once

#include <OCRA/Core.hpp>

#include <functional>

namespace OCRA::Command::Buffer {
struct ExecutionState;
}
namespace OCRA::Pipeline {
struct Impl {
    Impl(const PipelineBindingPoint& a_BindingPoint, const Layout::Handle& a_Layout)
        : layout(a_Layout)
        , bindingPoint(a_BindingPoint)
    {};
    virtual void Apply(Command::Buffer::ExecutionState&) const = 0;
    const Layout::Handle       layout;
    const PipelineBindingPoint bindingPoint;
};
}