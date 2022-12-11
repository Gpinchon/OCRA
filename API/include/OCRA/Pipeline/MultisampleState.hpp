/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:25:30
*/
#pragma once

#include <OCRA/Common/SampleCount.hpp>

#include <vector>

namespace OCRA::Pipeline::MultisampleState {
struct Info {
    SampleCount rasterizationSamples { SampleCount::Count1 };
    bool sampleShadingEnable { false };
    float minSampleShading { 1 };
    std::vector<uint32_t> sampleMasks;
    bool alphaToCoverageEnable { false };
    bool alphaToOneEnable { false };
};
}