/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:25:39
*/
#pragma once

#include <Handle.hpp>
#include <Pipeline/RasterizationState.hpp>

#include <GL/glew.hpp>

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
	case None :
		return GL_NONE;
	case Front :
		return GL_FRONT;
	case Back :
		return GL_BACK;
	case FrontAndBack :
		return GL_FRONT_AND_BACK;
	default:
		throw std::runtime_error("Unknown Cull Mode");
	}
}
static inline auto GetGLFrontFace(const FrontFace& a_FrontFace)
{
	switch (a_FrontFace)
	{
	case ClockWise :
		return GL_CW;
	case CounterClockWise:
		return GL_CCW;
	default :
		throw std::runtime_error("Unknown Front Face");
	}
}
struct Compile {
	Compile(const Device::Handle& a_Device, const Info& a_Info)
	    : rasterizerDiscardEnable(a_Info.rasterizerDiscardEnable)
		, depthClampEnable(a_Info.depthClampEnable)
		, depthBiasEnable(a_Info.depthBiasEnable)
		, depthBiasConstantFactor(a_Info.depthBiasConstantFactor)
		, depthBiasClamp(a_Info.depthBiasClamp)
		, depthBiasSlopeFactor(a_Info.depthBiasSlopeFactor)
		, lineWidth(a_Info.depthBiasSlopeFactor)
		, polygonOffsetMode(GetGLPolygonOffsetMode(a_Info.polygonMode));
		, polygonMode(GetGLPolygonMode(a_Info.polygonMode))
		, cullMode(GetGLCullMode(a_Info.cullMode))
		, frontFace(GetGLFrontFace(a_Info.frontFace))
	{}
	const GLboolean rasterizerDiscardEnable;
    const GLboolean depthClampEnable;
    const GLboolean depthBiasEnable;
    const GLfloat depthBiasConstantFactor;
    const GLfloat depthBiasClamp;
    const GLfloat depthBiasSlopeFactor;
    const GLfloat lineWidth;
	const GLenum polygonOffsetMode;
	const GLenum polygonMode;
	const GLenum cullMode;
	const GLenum frontFace;
	void operator()(void) {
		rasterizerDiscardEnable ? glEnable(GL_RASTERIZER_DISCARD) : glDisable(GL_RASTERIZER_DISCARD);
		depthClampEnable ? glEnable(GL_DEPTH_CLAMP) : glDisable(GL_DEPTH_CLAMP);
		depthBiasEnable ? glEnable(polygonOffsetMode) : glDisable(polygonOffsetMode);
		glPolygonOffsetClamp(
			depthBiasConstantFactor,
			depthBiasSlopeFactor,
			depthBiasClamp);
		glLineWidth(lineWidth);
		glPolygonMode(
			GL_FRONT_AND_BACK,
			polygonMode);
		if (cullMode != GL_NONE) {
			glEnable(GL_CULL_FACE);
			glCullFace(cullMode);
		}
		else glDisable(GL_CULL_FACE);
		glFrontFace(frontFace);
	}
};
static inline auto Default(const Device::Handle& a_Device)
{
	return Compile(a_Device, {});
}
}