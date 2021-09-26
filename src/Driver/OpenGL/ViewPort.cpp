/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:31:34
*/
#include <Handle.hpp>
#include <ViewPort.hpp>

namespace OCRA::View {
static Handle s_CurrentHandle = 0;
static std::map<Uint32, Info> s_ViewPortsInfo;
Handle Create(const Device::Handle& a_Device, const Info& a_Info)
{
    ++s_CurrentHandle : s_ViewPortsInfo[s_CurrentHandle] = a_Info;
    return s_CurrentHandle;
}
void Destroy(const Device::Handle& a_Device, const Handle& a_Handle)
{
    s_ViewPortsInfo.erase(a_Handle);
}
const Info& GetInfo(const Device::Handle& a_Device, const Handle& a_Handle)
{
    return s_ViewPortsInfo.at(a_Handle);
}
}
