/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:27:22
*/
#pragma once

#include <Handle.hpp>

#include <string>

HANDLE(OCRA::Window);

namespace OCRA::Window {
struct Info {
    uint16_t width { 0 }, height { 0 };
    std::string name;
};
Handle Create(const Info& a_Info);
}