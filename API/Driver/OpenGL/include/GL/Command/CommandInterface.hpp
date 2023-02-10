#pragma once

#include <functional>
#include <cassert>

namespace OCRA::Command::Buffer {
    struct ExecutionState;
}

namespace OCRA::Command
{
/**
* @brief the base for all commands
*/
struct CommandI {
    virtual ~CommandI() = 0; //pure virtual destructor to force destructor generation
    virtual void operator()(Buffer::ExecutionState&) = 0;
};

inline CommandI::~CommandI() {}

struct GenericCommand : CommandI {
    GenericCommand(std::function<void(Buffer::ExecutionState&)> a_Fun)
        : fun(a_Fun)
    {}
    virtual void operator()(Buffer::ExecutionState& a_ExecutionState) override {
        fun(a_ExecutionState);
    }
    std::function<void(Buffer::ExecutionState&)> fun;
};
}