/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:17
*/
#pragma once

#include <Device.hpp>
#include <Handle.hpp>

HANDLE(OCRA::Buffer::Vertex);

namespace OCRA::Buffer::Vertex {
struct Info {
	uint64_t size;
};
Handle Create(const Device::Handle& a_Device, const Info& a_Info);
}