#include <Shader/Module.hpp>

namespace OCRA::Shader::Module {
struct Impl
{
	Impl(const Device::Handle& a_Device, const Info& a_Info)
		: info(a_Info)
	{}
	const Info info;
};
Handle Create(const Device::Handle& a_Device, const Info& a_Info)
{
	return Handle(new Impl(a_Device, a_Info));
}
const Info& GetInfo(const Handle& a_Module)
{
	return a_Module->info;
}
}