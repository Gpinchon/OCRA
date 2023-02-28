#include <GL/Device.hpp>
#include <GL/Command/ExecutionState.hpp>
#include <GL/Pipeline/ShaderPipelineState.hpp>
#include <GL/Shader/Stage.hpp>

#include <GL/glew.h>

namespace OCRA::Pipeline {
CompileShaderPipelineState::CompileShaderPipelineState(
    const Device::Handle& a_Device,
    const PipelineShaderPipelineState& a_Info,
    const PipelineDynamicState&)
    : device(a_Device)
    , info(a_Info)
{
    a_Device->PushCommand([this, info = a_Info] {
        glGenProgramPipelines(1, &handle);
        for (const auto& stage : info.stages) {
            glUseProgramStages(handle, stage->stageBits, stage->handle);
        }
    }, true);
}
CompileShaderPipelineState::CompileShaderPipelineState(const CompileShaderPipelineState& a_Other)
    : device(a_Other.device)
    , info(a_Other.info)
    , handle(std::exchange(a_Other.handle, 0))
{}

CompileShaderPipelineState::~CompileShaderPipelineState()
{
    if (handle == 0) return; //this state set does not own a GL pipeline anymore
    device.lock()->PushCommand([handle = handle] {
        glDeleteProgramPipelines(1, &handle);
    }, false);
}
void CompileShaderPipelineState::operator()(Command::Buffer::ExecutionState& a_ExecutionState) const {
    glBindProgramPipeline(handle);
}
}