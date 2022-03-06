/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:25:39
*/
#pragma once

namespace OCRA::Pipeline::RasterizationState {
enum class PolygonMode {
    Fill,
    Line,
    Point,
    MaxValue
};
enum class CullMode {
	None,
    Front,
    Back,
    FrontAndBack,
    MaxValue
};
enum class FrontFace {
    Clockwise,
    CounterClockwise,
    MaxValue
};
struct Info {
    bool rasterizerDiscardEnable { false };
    bool depthClampEnable { true };
    bool depthBiasEnable { false };
    float depthBiasConstantFactor { 0 };
    float depthBiasSlopeFactor { 0 };
    float depthBiasClamp { 0 };
    float lineWidth { 1 };
    PolygonMode polygonMode { PolygonMode::Fill };
    CullMode cullMode { CullMode::Back };
    FrontFace frontFace { FrontFace::CounterClockwise };
};
}