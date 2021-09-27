/*
* @Author: gpinchon
* @Date:   2021-09-27 18:56:42
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-27 18:56:42
*/
#pragma once

#include <Handle.hpp>
#include <Pipeline/DepthStencilState.hpp>

#include <Driver/OpenGL/Stencil.hpp>
#include <Driver/OpenGL/Compare.hpp>

#include <GL/glew.h>

namespace OCRA::Pipeline::DepthStencilState {
	struct Impl {
		struct StencilOpState {
			StencilOpState(const DepthStencilState::StencilOpState& a_StencilOpState)
				: failOp(GetGLOperation(a_StencilOpState.failOp))
				, passOp(GetGLOperation(a_StencilOpState.passOp))
				, depthFailOp(GetGLOperation(a_StencilOpState.depthFailOp))
				, compareOp(GetGLOperation(a_StencilOpState.compareOp))
				, compareMask(a_StencilOpState.compareMask)
				, writeMask(a_StencilOpState.writeMask)
				, reference(a_StencilOpState.reference)
			{
			}
			const GLenum failOp; //the operation to be realized when stencil test FAILS
			const GLenum passOp; //the operation to be realized when stencil and depth test PASSES
			const GLenum depthFailOp; //the operation to be realized when stencil test PASSES but depth test FAILS
			const GLenum compareOp;
			const GLuint compareMask; //a mask that is ANDed with ref and the buffer's content
			const GLuint writeMask; //a mask that is ANDed with the stencil value about to be written to the buffer
			const GLuint reference; //the reference value used in comparison.
			void operator()(const GLenum& a_Face) const {
				glStencilOpSeparate(
					a_Face,
					failOp,
					depthFailOp,
					passOp);
				glStencilFuncSeparate(
					a_Face,
					compareOp,
					reference,
					compareMask);
				glStencilMaskSeparate(
					a_Face,
					writeMask);
			}
		};
		Impl(const DepthStencilState::Info& a_Info)
			: depthTestEnable(a_Info.depthTestEnable)
			, depthWriteEnable(a_Info.depthWriteEnable)
			, depthCompareOp(GetGLOperation(a_Info.depthCompareOp))
			, depthBoundsTestEnable(a_Info.depthBoundsTestEnable)
			, stencilTestEnable(a_Info.stencilTestEnable)
			, front(a_Info.front)
			, back(a_Info.back)
			, minDepthBounds(a_Info.minDepthBounds)
			, maxDepthBounds(a_Info.maxDepthBounds)
		{}
		const GLboolean depthTestEnable;
		const GLboolean depthWriteEnable;
		const GLenum depthCompareOp;
		const GLboolean depthBoundsTestEnable;
		const GLboolean stencilTestEnable;
		const StencilOpState front;
		const StencilOpState back;
		const GLfloat minDepthBounds;
		const GLfloat maxDepthBounds;
		void operator()(void) const {
			glDepthMask(depthWriteEnable);
			glDepthFunc(depthCompareOp);
			depthTestEnable ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
			depthBoundsTestEnable ? glEnable(GL_DEPTH_CLAMP) : glDisable(GL_DEPTH_CLAMP);
			stencilTestEnable ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
			front(GL_FRONT);
			back(GL_BACK);
			glDepthRangef(
				minDepthBounds,
				maxDepthBounds);
		}
	};
	//compiles the specified Vertex Input State into a callback
	inline auto Compile(const Device::Handle& a_Device, const Info& a_Info) {
		return Impl(a_Info);
	}
	inline auto Default(const Device::Handle& a_Device) {
		return Impl({});
	}
}