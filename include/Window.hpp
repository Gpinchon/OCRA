/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:27:22
*/
#pragma once

#include <Scalar.hpp>

#include <string>

namespace OCRA::Window {
struct Info {
    Uint16 width { 0 }, height { 0 };
    std::string name;
};
}