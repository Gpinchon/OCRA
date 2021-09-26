/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:27:03
*/
#pragma once

#include <Handle.hpp>

#include <vector>

namespace OCRA::Pass {
struct Info {
    FrameBuffer::Handle frameBuffer;
    Rect2D scissorTest;
    std::vector<State::Draw::Handle> drawStates;
    State::Clear::Handle clearState;
};
}