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
#define HANDLE(name)                                                        \
namespace name {                                                            \
    struct Impl;                                                            \
    typedef std::shared_ptr<Impl> Handle;                                   \
}