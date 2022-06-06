#include <GL/Pipeline/ShaderPipelineState.hpp>
#include <GL/Shader/Stage.hpp>
#include <GL/Device.hpp>
#include <GL/Command/ExecutionState.hpp>

#include <GL/glew.h>

namespace OCRA::Pipeline::ShaderPipelineState {
constexpr auto PushConstantBinding = 256;
Compile::Compile(const Device::Handle& a_Device, const Info& a_Info, const DynamicState::Info&)
	: device(a_Device)
	, info(a_Info)
{
	a_Device->PushCommand([this, info = a_Info] {
		glGenProgramPipelines(1, &handle);
		for (const auto& stage : info.stages) {
			glUseProgramStages(handle, stage->stageBits, stage->handle);
		}
		{
			GLenum props[] = { GL_BUFFER_BINDING };
			GLint   values[1024];
			GLsizei length = 0;
			glGetProgramResourceiv(
				handle,
				GL_SHADER_STORAGE_BLOCK,
				0,
				1,
				props,
				1024,
				&length,
				values);

		}
		glBindProgramPipeline(handle);
		glGenBuffers(1, &pushConstantHandle);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, pushConstantHandle);
		glBufferStorage(GL_SHADER_STORAGE_BUFFER, 128, nullptr, GL_DYNAMIC_STORAGE_BIT);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, PushConstantBinding, pushConstantHandle);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		glBindProgramPipeline(0);
	}, true);
}
Compile::Compile(const Compile& a_Other)
	: device(a_Other.device)
	, info(a_Other.info)
	, handle(std::exchange(a_Other.handle, 0))
	, pushConstantHandle(std::exchange(a_Other.pushConstantHandle, 0))
{}

Compile::~Compile()
{
	if (handle == 0) return; //this state set does not own a GL pipeline anymore
	device.lock()->PushCommand([handle = handle, pushConstantHandle = pushConstantHandle] {
		glDeleteProgramPipelines(1, &handle);
		glDeleteBuffers(1, &pushConstantHandle);
	}, false);
}
void Compile::operator()(Command::Buffer::ExecutionState& a_ExecutionState) const {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, pushConstantHandle);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER,
		a_ExecutionState.pushConstants.offset,
		a_ExecutionState.pushConstants.data.size(),
		a_ExecutionState.pushConstants.data.data());
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glBindProgramPipeline(handle);
}
}