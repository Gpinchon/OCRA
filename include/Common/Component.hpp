/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:28:28
*/
#pragma once

namespace OCRA::Component {
enum class Swizzle {
	Identity,
	Zero, One,
	R, G, B, A,
	MaxValue
};
struct Mapping {
	Swizzle r{ Swizzle::R };
	Swizzle g{ Swizzle::G };
	Swizzle b{ Swizzle::B };
	Swizzle a{ Swizzle::A };
};
}