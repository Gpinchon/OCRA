/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 15:03:29
*/
#pragma once

#include <Handle.hpp>

#include <bitset>

namespace OCRA::Buffer::Transfer
{
using AccessFlag = std::bitset<2>;
constexpr AccessFlag Read = 0b10;
constexpr AccessFlag Write = 0b01;
constexpr AccessFlag ReadWrite = 0b11;
struct Info
{
	Uint64 size{ 0 }; //specifies the size of the transfer buffer
	AccessFlag accessFlag{ ReadWrite }; //specified wether the transfer buffer is used to read, write or both
};
void* Map(const Device::Handle& a_Device, const Handle& a_Handle, Uint64 offset, Uint64 size);
void Unmap(const Device::Handle& a_Device, const Handle& a_Handle);
}