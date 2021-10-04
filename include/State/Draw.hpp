/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:15
*/
#pragma once

#include <Handle.hpp>
#include <Scalar.hpp>

namespace OCRA::State::Draw {
struct Info {
    constexpr auto MaxDescriptors = 32;
    struct {
        Pipeline::BlendingState::Handle blendingState;
        Pipeline::RasterizationState::Handle rasterizationState;
    } parameters;
    Pipeline::Graphics::Handle graphicsPipeline;
    Uint8 descriptorCount { 0 };
    Descriptor::Handle descriptors[MaxDescriptors];
};
}