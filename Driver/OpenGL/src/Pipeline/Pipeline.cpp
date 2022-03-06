#include <Handle.hpp>

#include <GL/Pipeline/Pipeline.hpp>

#include <map>
#include <memory>
#include <functional>

namespace OCRA::Pipeline {
static Handle s_CurrentHandle = 0;
static std::map<Handle, std::unique_ptr<Base>> s_Pipelines;
Base& Get(const Handle& a_Pipeline) {
	return *s_Pipelines.at(a_Pipeline);
}
Handle Create(const Device::Handle& a_Device, std::function<Base*()> a_Allocator) {
	++s_CurrentHandle;
	s_Pipelines.emplace(s_CurrentHandle, a_Allocator());
	return s_CurrentHandle;
}
void Destroy(const Device::Handle& a_Device, const Handle& a_Handle) {
	s_Pipelines.erase(a_Handle);
}
void Execute(const Device::Handle& a_Device, const Handle& a_Handle, Command::Buffer::ExecutionState& a_ExecutionState)
{
	Get(a_Handle).Execute(a_Device, a_Handle, a_ExecutionState);
}
}