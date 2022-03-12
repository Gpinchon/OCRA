/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/
#pragma once

#include <stdexcept>

namespace OCRA
{
enum class IndexType {
	None,
	Uint8,
	Uint16,
	Uint32,
	MaxValue
};
auto GetIndexTypeSize(const IndexType& a_IndexType){
	switch (a_IndexType) {
	case Uint8 :
		return 1;
	case Uint16 :
		return 2;
	case Uint32 :
		return 4;
	default : throw std::runtime_error("Unknown Index Type");
	}
}
}