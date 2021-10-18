/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 15:03:53
*/
#include <Handle.hpp>
#include <Scalar.hpp>

#include <functional>
#include <array>

namespace OCRA::Pipeline::Graphics
{
struct BufferBindings {
	static constexpr auto MaxVertexBuffers = 32;
	struct {
		Buffer::Vertex::Handle indexBuffer;
		Uint64 offset;
		IndexType indexType;
	} IBO;
	struct {
		Uint32 firstBinding;
		Uint32 bindingCount;
		std::array<Buffer::Vertex::Handle, MaxVertexBuffers> vertexBuffers;
		std::array<Uint64, MaxVertexBuffers> offsets;
	} VBO;
};
std::function<void()> CompileCommand(const Handle& a_Handle, const BufferBindings& a_BufferBindings);
}