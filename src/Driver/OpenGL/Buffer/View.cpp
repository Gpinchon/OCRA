/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:31:44
*/
#include <Buffer/View.hpp>
#include <Handle.hpp>

#include <map>

namespace OCRA::Buffer::View {
static Handle s_CurrentHandle = 0;
static std::map<Handle, Info> s_BufferViewsInfo;
Handle Create(const Device::Handle& a_Device, const Info& a_Info)
{
    ++s_CurrentHandle;
    s_BufferViewsInfo[s_CurrentHandle] = a_Info;
    return s_CurrentHandle;
}
void Destroy(const Device::Handle& a_Device, const Handle& a_Handle)
{
    s_BufferViewsInfo.erase(a_Handle);
}
const Info& GetInfo(const Device::Handle& a_Device, const Handle& a_Handle)
{
    return s_BufferViewsInfo.at(a_Handle);
}
}