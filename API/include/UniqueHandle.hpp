#pragma once

#include <memory>

// Handle uses Pimpl pattern to hide real implementation
// Handles are not convertible between object types
// "Unique" handles are unique_ptr in disguise
#define OCRA_DECLARE_UNIQUE_HANDLE(name)      \
namespace name {                              \
    struct Impl;                              \
    typedef std::unique_ptr<Impl> WeakHandle; \
}