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
struct VertexBufferBindings {
	static constexpr auto MaxVertexBuffers = 32;
	Uint32 firstBinding;
	Uint32 bindingCount;
	std::array<Buffer::Vertex::Handle, MaxVertexBuffers> vertexBuffers;
	std::array<Uint64, MaxVertexBuffers> offsets;
};
std::function<void()> CompileCommand(const Handle& a_Handle, const VertexBufferBindings& a_BufferBindings);
}