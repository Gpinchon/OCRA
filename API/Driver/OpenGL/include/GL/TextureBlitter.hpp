#pragma once

#include <OCRA/Core.hpp>

#include <GL/glew.h>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA
{
struct TextureBlitter
{
	TextureBlitter(Device::Impl& a_Device);
	~TextureBlitter();
	void Blit3D(
		const Image::Handle& a_SrcImage,
		const Image::Handle& a_DstImage,
		const ImageBlit& a_Blit,
		const Filter& a_Filter) const;
	void Blit(
		const Image::Handle& a_SrcImage,
		const Image::Handle& a_DstImage,
		const size_t a_BlitsCount,
		const ImageBlit* a_Blits,
		const Filter& a_Filter) const;
	void Blit(
		const Image::Handle& a_SrcImage,
		const Image::Handle& a_DstImage,
		const std::vector<ImageBlit>& a_Blits,
		const Filter& a_Filter) const;
	Device::Impl& device;
	std::array<GLuint, 2> frameBuffers{ 0, 0 };
	//For 3D Texture, a Compute Shader is used
	GLuint shaderProgram = 0;
	GLuint samplerSrc = 0;
};
}