#include <PhysicalDevice.hpp>

namespace OCRA::PhysicalDevice
{
struct Impl
{
	Impl() {}
	Properties properties;
};
Handle Create()
{
	return Handle(new Impl());
}
const Properties& GetProperties(const Handle& a_PhysicalDevice)
{
	return a_PhysicalDevice->properties;
}
}