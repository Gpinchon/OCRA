/*
 * @Author: gpinchon
 * @Date:   2021-09-26 00:00:00
 * @Last Modified by:   gpinchon
 * @Last Modified time: 2021-09-26 14:26:50
 */
#pragma once

#include <memory>

// Handle uses Pimpl pattern to hide real implementation
// Handles are not convertible between object types
#define OCRA_DECLARE_HANDLE(name)         \
    namespace name {                      \
    struct Impl;                          \
    typedef std::shared_ptr<Impl> Handle; \
    }

// Handle uses Pimpl pattern to hide real implementation
// Handles are not convertible between object types
// "Unique" handles are unique_ptr in disguise
#define OCRA_DECLARE_UNIQUE_HANDLE(name)      \
    namespace name {                          \
    struct Impl;                              \
    typedef std::unique_ptr<Impl> WeakHandle; \
    }

// Handle uses Pimpl pattern to hide real implementation
// Handles are not convertible between object types
// "Weak" handles are weak_ptr in disguise
#define OCRA_DECLARE_WEAK_HANDLE(name)      \
    namespace name {                        \
    struct Impl;                            \
    typedef std::weak_ptr<Impl> WeakHandle; \
    }
