/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 15:03:34
*/

#include <Handle.hpp>
#include <Pipeline/ShaderPipelineState.hpp>

#include <GL/Pipeline/ShaderPipelineState.hpp>
#include <GL/Pipeline/ShaderPipelinePool.hpp>
#include <GL/glew.h>

#include <stdexcept>
#include <map>

namespace OCRA::Pipeline::ShaderPipelineState {
static std::map<Device::Handle, ShaderPipelinePool> s_ShaderPipelinesPools;
Compile::Compile(const Device::Handle& a_Device, const Info& a_Info)
{
	auto& shaderPipelinesPool { s_ShaderPipelinesPools[a_Device] };
	if (auto shaderPipeline { shaderPipelinesPool.FindSimilar(a_Info) }; shaderPipeline.Get() != nullptr) //try to find a similar Pipeline
		shaderPipelineRef = shaderPipeline;
    else if (auto shaderPipeline { shaderPipelinesPool.FindFree() }; shaderPipeline.Get() != nullptr) //we couldn't find similar Pipeline, try to find free Pipeline
	{
        shaderPipeline->Set(a_Device, a_Info);
		shaderPipelineRef = shaderPipeline;
    }
    else throw std::runtime_error("No more free Pipeline");
}
}