/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:25:30
*/
#pragma once

#include <Common/SampleCount.hpp>

#include <array>

namespace OCRA::Pipeline::MultisampleState {
struct Info {
    SampleCount rasterizationSamples { SampleCount::Count1 };
    bool sampleShadingEnable { false };
    float minSampleShading { 1 };
    std::array<uint32_t, 32> sampleMasks; //sampleMask size == rasterizationSamples / 32
    bool alphaToCoverageEnable { false };
    bool alphaToOneEnable { false };
};
}