#include <GL/Pipeline/ShaderPipelineState.hpp>
#include <GL/Shader/Stage.hpp>
#include <GL/Device.hpp>

#include <GL/glew.h>

namespace OCRA::Pipeline::ShaderPipelineState {
Compile::Compile(const Device::Handle& a_Device, const Info& a_Info, const DynamicState::Info&)
	: device(a_Device)
	, info(a_Info)
{
	a_Device->PushCommand([this, info = a_Info] {
		glGenProgramPipelines(1, &handle);
		for (const auto& stage : info.stages) {
			const auto& stageInfo{ Shader::Stage::GetInfo(stage) };
			const auto& stageGLHandle{ Shader::Stage::GetGLHandle(stage) };
			glUseProgramStages(handle, Shader::Stage::GetGLStage(stageInfo.stage), stageGLHandle);
		}
	}, true);
}
Compile::Compile(const Compile& a_Other)
	: device(a_Other.device)
	, info(a_Other.info)
	, handle(std::exchange(a_Other.handle, 0))
{}

Compile::~Compile()
{
	if (handle == 0) return;
	device.lock()->PushCommand([handle = handle] {
		glDeleteProgramPipelines(1, &handle);
	}, false);
}
void Compile::operator()(Command::Buffer::ExecutionState&) const {
	glBindProgramPipeline(handle);
}
}