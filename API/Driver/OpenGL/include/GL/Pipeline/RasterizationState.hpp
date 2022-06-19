/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:25:39
*/
#pragma once

#include <Handle.hpp>
#include <Pipeline/RasterizationState.hpp>
#include <Pipeline/DynamicState.hpp>

#include <GL/Command/ExecutionState.hpp>
#include <GL/glew.h>

#include <stdexcept>

namespace OCRA::Pipeline::RasterizationState {
static inline auto GetGLPolygonMode(const PolygonMode& a_Mode)
{
	switch (a_Mode)
	{
	case PolygonMode::Fill :
		return GL_FILL;
	case PolygonMode::Line :
		return GL_LINE;
	case PolygonMode::Point :
		return GL_POINT;
	default:
		throw std::runtime_error("Unknown Polygon Mode");
	}
}
static inline auto GetGLPolygonOffsetMode(const PolygonMode& a_Mode)
{
	switch (a_Mode)
	{
	case PolygonMode::Fill :
		return GL_POLYGON_OFFSET_FILL;
	case PolygonMode::Line :
		return GL_POLYGON_OFFSET_LINE;
	case PolygonMode::Point :
		return GL_POLYGON_OFFSET_POINT;
	default:
		throw std::runtime_error("Unknown Polygon Mode");
	}
}
static inline auto GetGLCullMode(const CullMode& a_Mode)
{
	switch (a_Mode)
	{
	case CullMode::None :
		return GL_NONE;
	case CullMode::Front :
		return GL_FRONT;
	case CullMode::Back :
		return GL_BACK;
	case CullMode::FrontAndBack :
		return GL_FRONT_AND_BACK;
	default:
		throw std::runtime_error("Unknown Cull Mode");
	}
}
static inline auto GetGLFrontFace(const FrontFace& a_FrontFace)
{
	switch (a_FrontFace)
	{
	case FrontFace::Clockwise :
		return GL_CW;
	case FrontFace::CounterClockwise:
		return GL_CCW;
	default :
		throw std::runtime_error("Unknown Front Face");
	}
}
struct GLInfo {
	GLInfo(const Info& a_Info)
	: rasterizerDiscardEnable(a_Info.rasterizerDiscardEnable)
	, depthClampEnable(a_Info.depthClampEnable)
	, depthBiasEnable(a_Info.depthBiasEnable)
	, depthBiasConstantFactor(a_Info.depthBiasConstantFactor)
	, depthBiasClamp(a_Info.depthBiasClamp)
	, depthBiasSlopeFactor(a_Info.depthBiasSlopeFactor)
	, lineWidth(a_Info.lineWidth)
	, polygonOffsetMode(GetGLPolygonOffsetMode(a_Info.polygonMode))
	, polygonMode(GetGLPolygonMode(a_Info.polygonMode))
	, cullMode(GetGLCullMode(a_Info.cullMode))
	, frontFace(GetGLFrontFace(a_Info.frontFace))
	{}
    GLboolean rasterizerDiscardEnable { false };
    GLboolean depthClampEnable { true };
    GLboolean depthBiasEnable { false };
    GLfloat depthBiasConstantFactor { 0 };
    GLfloat depthBiasClamp { 0 };
    GLfloat depthBiasSlopeFactor { 0 };
    GLfloat lineWidth { 1 };
	GLenum polygonOffsetMode{ GL_POLYGON_OFFSET_FILL };
    GLenum polygonMode { GL_FILL };
    GLenum cullMode { GL_BACK };
    GLenum frontFace { GL_CCW };
};
inline auto Compile(const Device::Handle& a_Device, const Info& a_Info, const DynamicState::Info& a_DynamicState)
{
	return [
		info(GLInfo(a_Info)),
		dynamicRasterizerDiscardEnable(a_DynamicState.Contains(DynamicState::State::RasterizerDiscardEnable)),
		dynamicLineWidth(a_DynamicState.Contains(DynamicState::State::LineWidth)),
		dynamicCullMode(a_DynamicState.Contains(DynamicState::State::CullMode)),
		dynamicFrontFace(a_DynamicState.Contains(DynamicState::State::FrontFace)),
		dynamicDepthBiasEnable(a_DynamicState.Contains(DynamicState::State::DepthBiasEnable)),
		dynamicDepthBias(a_DynamicState.Contains(DynamicState::State::DepthBias))
	](Command::Buffer::ExecutionState& a_ExecutionState){
		const auto &bRasterizerDiscardEnable = dynamicRasterizerDiscardEnable ? a_ExecutionState.dynamicStates.rasterizerDiscardEnable : info.rasterizerDiscardEnable;
		const auto &bDepthBiasEnable = dynamicDepthBiasEnable ? a_ExecutionState.dynamicStates.depthBiasEnable : info.depthBiasEnable;
		const auto &fDepthBiasConstantFactor = dynamicDepthBias ? a_ExecutionState.dynamicStates.depthBiasConstantFactor : info.depthBiasConstantFactor;
		const auto &fDepthBiasSlopeFactor = dynamicDepthBias ? a_ExecutionState.dynamicStates.depthBiasSlopeFactor : info.depthBiasSlopeFactor;
		const auto &fDepthBiasClamp = dynamicDepthBias ? a_ExecutionState.dynamicStates.depthBiasClamp : info.depthBiasClamp;
		const auto &fLineWidth = dynamicLineWidth ? a_ExecutionState.dynamicStates.lineWidth : info.lineWidth;
		const auto &eFrontFace = dynamicFrontFace ? a_ExecutionState.dynamicStates.frontFace : info.frontFace;
		const auto &eCullMode = dynamicCullMode ? a_ExecutionState.dynamicStates.cullMode : info.cullMode;
		bRasterizerDiscardEnable ? glEnable(GL_RASTERIZER_DISCARD) : glDisable(GL_RASTERIZER_DISCARD);
		info.depthClampEnable ? glEnable(GL_DEPTH_CLAMP) : glDisable(GL_DEPTH_CLAMP);
		bDepthBiasEnable ? glEnable(info.polygonOffsetMode) : glDisable(info.polygonOffsetMode);
		glPolygonMode(
			GL_FRONT_AND_BACK,
			info.polygonMode);
		glPolygonOffsetClamp(
			fDepthBiasConstantFactor,
			fDepthBiasSlopeFactor,
			fDepthBiasClamp);
		glLineWidth(fLineWidth);
		glFrontFace(eFrontFace);
		if (eCullMode != GL_NONE) {
			glEnable(GL_CULL_FACE);
			glCullFace(eCullMode);
		}
		else glDisable(GL_CULL_FACE);
	};
}
static inline auto Default(const Device::Handle& a_Device)
{
	return Compile(a_Device, {}, {});
}
}