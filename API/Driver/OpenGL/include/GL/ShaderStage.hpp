/*
* @Author: gpinchon
* @Date:   2021-10-04 20:24:27
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-04 20:25:30
*/
#pragma once

#include <OCRA/Core.hpp>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Shader {
struct Stage {
    Stage(const Device::Handle& a_Device, const PipelineShaderStage& a_Info);
    Stage(Stage&& a_Stage) noexcept
    {
        std::swap(device,    a_Stage.device);
        std::swap(handle,    a_Stage.handle);
        std::swap(stage,     a_Stage.stage);
        std::swap(stageBits, a_Stage.stageBits);
    }
    Stage(const Stage& a_Other) = delete;
    ~Stage();
    Device::WeakHandle device;
    uint32_t handle;
    uint32_t stage;
    uint32_t stageBits;
};
}