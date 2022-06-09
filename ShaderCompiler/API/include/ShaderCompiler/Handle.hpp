#pragma once

#include <memory>

// Handle uses Pimpl pattern to hide real implementation
// Handles are not convertible between object types
#define OCRA_DECLARE_HANDLE(name)           \
namespace name {                            \
    struct Impl;                            \
    typedef std::shared_ptr<Impl> Handle;   \
}