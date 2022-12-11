/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:27:27
*/
#pragma once

#include <OCRA/Handle.hpp>

/**
 * @brief Describes shader ressources
 */
namespace OCRA::Descriptor {
struct Info {
    constexpr auto MaxLayouts = 32;
    const Pool::Handle descriptorPool;
    uint8_t layoutCount { 0 };
    Layout::Handle layouts[MaxLayouts];
}
}