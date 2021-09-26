/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:25:30
*/
#pragma once

#include <Scalar.hpp>

namespace OCRA::Pipeline::MultisampleState {
enum class Sample {
    Count1 = 1,
    Count2 = 2,
    Count4 = 4,
    Count8 = 8,
    Count16 = 16,
    Count32 = 32,
    Count64 = 64,
    MaxValue
};
struct Info {
    Sample rasterizationSamples { Sample::Count1 };
    bool sampleShadingEnable { false };
    float minSampleShading { 1 };
    Uint32 sampleMasks[32]; //sampleMask size == rasterizationSamples / 32
    bool alphaToCoverageEnable { false };
    bool alphaToOneEnable { false };
};
}