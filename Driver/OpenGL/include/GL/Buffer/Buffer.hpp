/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:17
*/
#pragma once

#include <Scalar.hpp>

#include <GL/glew.h>

namespace OCRA::Buffer {
struct Impl {
	Impl()
	{
		glCreateBuffers(1, &handle);
	}
	~Impl()
	{
		glDeleteBuffers(1, &handle);
	}
	GLuint handle{ 0 };
};
}