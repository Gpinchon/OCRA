/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-03 22:11:23
*/
#pragma once

#include <Handle.hpp>
#include <Pipeline/ShaderPipelineState.hpp>

#include <GL/Shader/Stage.hpp>
#include <GL/ObjectPool.hpp>
#include <GL/glew.h>

namespace OCRA::Pipeline::ShaderPipelineState {
struct ShaderPipeline
{
	ShaderPipeline()
	{
		glGenProgramPipelines(1, &handle);
	}
	~ShaderPipeline()
	{
		glDeleteProgramPipelines(1, &handle);
	}
	inline void Reset() noexcept
    {
        info = {};
        glUseProgramStages(handle, 0, GL_ALL_SHADER_BITS);
    }
	inline void Set(const Device::Handle& a_Device, const Info& a_Info)
	{
		info = a_Info;
		for (auto i = 0u; i < info.stageCount; ++i) {
			const auto& stageHandle{ info.stages.at(i) };
			const auto& stageInfo{ Shader::Stage::GetInfo(a_Device, stageHandle) };
			const auto& stageGLHandle{ Shader::Stage::GetGLHandle(a_Device, stageHandle) };
            glUseProgramStages(handle, Shader::Stage::GetGLStage(stageInfo.stage), stageGLHandle);
		}
	}
	Info info;
    GLuint handle { 0 };
};
static inline bool operator==(const Info& a_lInfo, const Info& a_rInfo) noexcept
{
	if (a_lInfo.stagesCount != a_rInfo.stagesCount) return false;
	for (auto i = 0u; i < a_lInfo.stagesCount; ++i)
	{
		if (a_lInfo.stages.at(i) != a_rInfo.stages.at(i)) return false;
	}
	return true;
}
struct ShaderPipelinePool : public ObjectPool<ShaderPipeline>
{
    inline Reference FindSimilar(const Info& a_Info) noexcept
    {
        for (auto& pipelineIndex : usedIndices) {
            if (objectArray.at(pipelineIndex).info == a_Info)
                return Reference(*this, pipelineIndex);
        }
        return Reference(*this, -1);
    }
    inline virtual void ReleaseObject(IndexType a_Index) override {
        Get(a_Index).Reset();
        ObjectPool<ShaderPipeline>::ReleaseObject(a_Index);
    }
};
}