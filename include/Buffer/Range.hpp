/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 15:03:29
*/
#pragma once

#include <Handle.hpp>

#include <bitset>

namespace OCRA::Buffer
{
struct Range
{
	uint64_t offset{ 0 }, size{ 0 };
};
}