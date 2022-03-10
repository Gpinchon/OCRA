/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:31:34
*/
#include <Device.hpp>
#include <Handle.hpp>
#include <ViewPort.hpp>

namespace OCRA::ViewPort {
struct Impl {
    Impl(const Info& a_Info) : info(a_Info) {};
    Info info;
};
Handle Create(const Device::Handle& a_Device, const Info& a_Info)
{
    return Handle(new Impl(a_Info));
}
const Info& GetInfo(const Device::Handle& a_Device, const Handle& a_Handle)
{
    return a_Handle->info;
}
}
