/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/
#pragma once

#include <Handle.hpp>
#include <Pipeline/Graphics.hpp>
#include <Command/RenderPass.hpp>

#include <GL/Pipeline/Graphics.hpp>
#include <GL/glew.hpp>

#include <cassert>
#include <functional>
#include <mutex>

namespace OCRA::Command
{
struct VertexBufferBinding {
	static constexpr auto MaxVertexBuffers = 32;
	Uint32 firstBinding{ 0 };
	Uint32 bindingCount{ 0 };
	std::array<GLuint, MaxVertexBuffers> vertexBuffers{ 0 };
	std::array<Uint64, MaxVertexBuffers> offsets{ 0 };
};
struct IndexBufferBinding {
	GLuint buffer{ 0 };
	Uint64 offset{ 0 };
	GLenum indexType{ GL_NONE };
};
struct RenderPass
{
	void StartRecording(const RenderPassBeginInfo& a_BeginInfo)
	{
		recording.lock();
		beginInfo = a_BeginInfo;
	}
	void EndRecording()
	{
		recording.unlock();
	}
	std::function<void()> CompileGraphicStates(const Device::Handle& a_Device) {
		if (!compiled)
		{
			assert(graphicsPipeline != 0);
			auto graphicsPipelineState = Pipeline::Graphics::Compile(a_Device, graphicsPipeline);
			const auto& graphicsPipelineInfo{ Pipeline::Graphics::GetInfo(graphicsPipeline) };
			const auto& vertexInputInfo{ graphicsPipelineInfo.vertexInputState };
			auto vertexBindingState = [vertexBinding = vertexBinding, bindingDescriptions = vertexInputInfo.bindingDescriptions](){
				for (auto i = 0u; i < vertexBinding.bindingCount; ++i)
				{
					const auto bindingIndex{ i + vertexBinding.firstBinding };
					const auto& binding{ bindingDescriptions.at(bindingIndex) };
					const auto& buffer{ vertexBinding.vertexBuffers.at(i) };
					const auto& offset{ vertexBinding.offsets.at(i) };
					/*glVertexArrayVertexBuffer(
		                handle,
		                binding.binding,
		                buffer,
		                offset,
		                binding.stride);*/
					glBindVertexBuffer(
						binding.binding,
						buffer,
						offset,
						binding.stride);
				}
			};
			compiled = true;
		}
		return [graphicsPipelineState = graphicsPipelineState, vertexBindingState = vertexBindingState](){
			graphicsPipelineState();
			vertexBindingState();
		};
	}
	std::function<void()> CompileDrawingCommands(const Device::Handle& a_Device) {

	}
	bool compiled{ false };
	RenderPassBeginInfo beginInfo;
	Pipeline::Graphics::Handle graphicsPipeline{ 0 };
	VertexBufferBindings vertexBinding;
	IndexBufferBinding indexBuffer;
	std::mutex recording;
};
}