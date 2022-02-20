/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:27:08
*/
#pragma once

#include <Scalar.hpp>

namespace OCRA {
struct Rect2D {
    struct {
        float x { 0 }, y { 0 };
    } offset;
    struct {
        Uint16 width { 0 }, height { 0 };
    } extent;
};
}