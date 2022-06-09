/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:28:07
*/
#pragma once

#include <Handle.hpp>

namespace OCRA::Image::Descriptor {
struct Info {
    Sampler::Handle sampler;
    Image::View::Handle imageView;
};
}