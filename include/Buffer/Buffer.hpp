/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:27:24
*/
#pragma once

#include <Scalar.hpp>

#include <bitset>

namespace OCRA::Buffer {
enum class Usage {
    TransferSrc,
    TransferDst,
    UniformTexelBuffer,
    StorageTexelBuffer,
    UniformBuffer,
    StorageBuffer,
    IndexBuffer,
    VertexBuffer,
    IndirectBuffer
};
using UsageFlags = std::bitset<7>;
constexpr auto Dynamic = 0x1000000;
constexpr auto Read = 0x0100000;
constexpr auto Write = 0x0010000;
constexpr auto Persistent = 0x0000100;
constexpr auto Coherent = 0x0000010;
constexpr auto Client = 0x0000001;
constexpr auto Default = 0x0000000;
struct Info {
    UsageFlags usage { Default };
    Uint64 size { 0 };
};
}