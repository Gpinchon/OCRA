#pragma once

#include <memory>

// Handle uses Pimpl pattern to hide real implementation
// Handles are not convertible between object types
// "Weak" handles are weak_ptr in disguise
#define OCRA_DECLARE_WEAK_HANDLE(name)      \
namespace name {                            \
    struct Impl;                            \
    typedef std::weak_ptr<Impl> WeakHandle; \
}