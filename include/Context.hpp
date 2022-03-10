/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:32
*/
#pragma once

#include <Handle.hpp>
#include <Window.hpp>

#include <string>

HANDLE(OCRA::Context);

namespace OCRA::Context {
struct Info {
    Window::Handle window;
    std::string version;
};
Handle Create(const Info& a_Info);
}
