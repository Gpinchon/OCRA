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
	void Blit(
		const Image::Handle& a_SrcImage,
		const Image::Handle& a_DstImage,
		const std::vector<ImageBlit>& a_Blits,
		const Filter& a_Filter) const;
	Device::Impl& device;
	GLuint shaderProgram = 0;
	GLuint samplerSrc = 0;
};
}