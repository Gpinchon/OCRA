/*
 * @Author: gpinchon
 * @Date:   2021-09-26 00:00:00
 * @Last Modified by:   gpinchon
 * @Last Modified time: 2021-09-26 14:25:30
 */
#pragma once

#include <OCRA/Core.hpp>

#include <array>

#include <GL/glew.h>

namespace OCRA::Command::Buffer {
struct ExecutionState;
}

namespace OCRA::Pipeline {
struct CompileMultisampleState {
    CompileMultisampleState(const Device::Handle& a_Device, const PipelineMultisampleState& a_Info, const PipelineDynamicState&)
        : sampleShadingEnable(a_Info.sampleShadingEnable)
        , minSampleShading(a_Info.minSampleShading)
        , sampleMasks(a_Info.sampleMasks)
        , alphaToCoverageEnable(a_Info.alphaToCoverageEnable)
        , alphaToOneEnable(a_Info.alphaToOneEnable)
        , rasterizationSamples(uint8_t(a_Info.rasterizationSamples))
    {
    }
    void operator()(Command::Buffer::ExecutionState&) const
    {
        sampleShadingEnable ? glEnable(GL_SAMPLE_SHADING) : glDisable(GL_SAMPLE_SHADING);
        glMinSampleShading(minSampleShading);
        for (auto i = 0; i < sampleMasks.size(); ++i)
            glSampleMaski(i, sampleMasks.at(i));
        alphaToCoverageEnable ? glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE) : glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
        alphaToOneEnable ? glEnable(GL_SAMPLE_ALPHA_TO_ONE) : glDisable(GL_SAMPLE_ALPHA_TO_ONE);
        // extension : GL_EXT_raster_multisample
        rasterizationSamples > 1 ? glEnable(GL_RASTER_MULTISAMPLE_EXT) : glDisable(GL_RASTER_MULTISAMPLE_EXT);
        glRasterSamplesEXT(rasterizationSamples, false);
    }
    const bool sampleShadingEnable;
    const float minSampleShading;
    const std::vector<uint32_t> sampleMasks;
    const bool alphaToCoverageEnable;
    const bool alphaToOneEnable;
    const uint8_t rasterizationSamples;
};
}
