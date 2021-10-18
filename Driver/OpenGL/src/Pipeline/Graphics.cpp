/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 15:03:53
*/
#include <Handle.hpp>
#include <Pipeline/Graphics.hpp>

#include <functional>
#include <map>
#include <vector>

#include <GL/Pipeline/ColorBlendState.hpp>
#include <GL/Pipeline/DepthStencilState.hpp>
#include <GL/Pipeline/MultisampleState.hpp>
#include <GL/Pipeline/ShaderPipelineState.hpp>
#include <GL/Pipeline/TessellationState.hpp>
#include <GL/Pipeline/ViewPortState.hpp>
#include <GL/Pipeline/Graphics.hpp>

#include <GL/glew.h>

namespace OCRA::Pipeline::Graphics {
struct Impl {
	Impl(const Device::Handle& a_Device, const Info& a_Info)
	: info(a_Info)
	, colorBlendState(ColorBlendState::Compile(a_Device, a_Info.colorBlendState))
	, depthStencilState(DepthStencilState::Compile(a_Device, a_Info.depthStencilState))
	, multisampleState(MultisampleState::Compile(a_Device, a_Info.multiSampleState))
	, shaderPipelineState(ShaderPipelineState::Compile(a_Device, a_Info.shaderPipelineState))
	, tessellationState(TessellationState::Compile(a_Device, a_Info.tessellationState))
	, viewportState(ViewPortState::Compile(a_Device, a_Info.viewPortState))
	{}
	void operator()()
	{
		colorBlendState();
		depthStencilState();
		multisampleState();
		shaderPipelineState();
		tessellationState();
		viewportState();
	}
    const Info info;
    const std::function<void()> colorBlendState;
    const std::function<void()> depthStencilState;
	const std::function<void()> multisampleState;
	const std::function<void()> shaderPipelineState;
	const std::function<void()> tessellationState;
	const std::function<void()> viewportState;
};
static Handle s_CurrentHandle = 0;
static std::map<Handle, Impl> s_GraphicsPipelines;
Handle Create(const Device::Handle& a_Device, const Info& a_Info)
{
    ++s_CurrentHandle;
    s_GraphicsPipelines.emplace(s_CurrentHandle, Impl(a_Device, a_Info));
    return s_CurrentHandle;
}
void Destroy(const Device::Handle& a_Device, const Handle& a_Handle)
{
    s_GraphicsPipelines.erase(a_Handle);
}
const Info& GetInfo(const Device::Handle& a_Device, const Handle& a_Handle)
{
    return s_GraphicsPipelines.at(a_Handle).info;
}
struct GLBufferBindings {
	static constexpr auto MaxVertexBuffers = 32;
	struct {
		GLuint indexBuffer;
		Uint64 offset;
		IndexType indexType;
	} IBO;
	struct {
		Uint32 firstBinding;
		Uint32 bindingCount;
		std::array<GLuint, MaxVertexBuffers> vertexBuffers;
		std::array<Uint64, MaxVertexBuffers> offsets;
	} VBO;
};
std::function<void()> CompileCommand(const Handle& a_Handle, const BufferBindings& a_BufferBindings)
{
	const auto& impl{ s_GraphicsPipelines.at(a_Handle) };
	const auto& info{ impl.info.vertexInputState };
	GLBufferBindings bindings;
	bindings.VBO.offsets = a_BufferBindings.VBO.offsets;
	bindings.VBO.firstBinding = a_BufferBindings.VBO.firstBinding;
	bindings.VBO.bindingCount = a_BufferBindings.VBO.bindingCount;
	for (auto vboIndex = a_BufferBindings.VBO.firstBinding; vboIndex < a_BufferBindings.VBO.bindingCount; ++vboIndex) {
		auto bufferHandle{ Buffer::Vertex::GetBufferHandle(a_Device, binding.buffer) };
		bindings.VBO.vertexBuffers.at(vboIndex) = Buffer::GetGLHandle(a_Device, bufferHandle);
	}
	return [info = info, bindings = bindings](){
		impl();
		for (auto attribIndex = 0u; attribIndex < info.attributeDescriptionCount; ++attribIndex) {
            const auto& attribute { info.attributeDescriptions.at(attribIndex) };
            /*glEnableVertexArrayAttrib(
                handle,
                attribIndex);
            glVertexArrayAttribFormat(
                handle,
                attribIndex,
                attribute.format.valuesPerVertex,
                GetGLFormatType(attribute.format.type),
                attribute.format.normalized,
                attribute.format.relativeOffset);*/
            const auto& binding { info.bindingDescriptions.at(attribute.bindingIndex) };
            /*glVertexArrayAttribBinding(
                handle,
                attribIndex,
                attribute.bindingIndex);*/
			auto bufferHandle{ Buffer::Vertex::GetBufferHandle(a_Device, binding.buffer) };
			auto bufferGLHandle{ Buffer::GetGLHandle(a_Device, bufferHandle) };
            /*glVertexArrayVertexBuffer(
                handle,
                attribute.bindingIndex,
                bufferGLHandle,
                binding.offset,
                binding.stride);*/
        }
	};
}
}