#pragma once

#include <OCRA/Core.hpp>

#include <functional>

namespace OCRA::Command::Buffer {
struct ExecutionState;
}

namespace OCRA::Pipeline {
struct Impl {
    Impl(const PipelineBindingPoint& a_BindingPoint)
        : bindingPoint(a_BindingPoint) {};
    virtual void Apply(Command::Buffer::ExecutionState&) const = 0;
    const PipelineBindingPoint bindingPoint;
};
}
