/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:25:30
*/
#pragma once

#include <Pipeline/MultisampleState.hpp>
#include <Scalar.hpp>

#include <array>

#include <GL/glew.h>

namespace OCRA::Pipeline::DynamicState {
struct Info;
}

namespace OCRA::Pipeline::MultisampleState {
const auto Compile(const Device::Handle& a_Device, const Info& a_Info, const DynamicState::Info&)
{
	return [
		sampleShadingEnable = a_Info.sampleShadingEnable,
		minSampleShading = a_Info.minSampleShading,
		sampleMasks = a_Info.sampleMasks,
		alphaToCoverageEnable = a_Info.alphaToCoverageEnable,
		alphaToOneEnable = a_Info.alphaToOneEnable,
		rasterizationSamples = Uint8(a_Info.rasterizationSamples)
	]() {
		sampleShadingEnable ? glEnable(GL_SAMPLE_SHADING) : glDisable(GL_SAMPLE_SHADING);
		glMinSampleShading(minSampleShading);
		for (auto i = 0; i < sampleMasks.size(); ++i) glSampleMaski(i, sampleMasks.at(i));
		alphaToCoverageEnable ? glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE) : glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		alphaToOneEnable ? glEnable(GL_SAMPLE_ALPHA_TO_ONE) : glDisable(GL_SAMPLE_ALPHA_TO_ONE);
		//extension : GL_EXT_raster_multisample
		rasterizationSamples > 1 ? glEnable(GL_RASTER_MULTISAMPLE_EXT) : glDisable(GL_RASTER_MULTISAMPLE_EXT);
		glRasterSamplesEXT(rasterizationSamples, false);
	};
}
}