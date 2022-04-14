#pragma once

#include <Handle.hpp>

#include <GL/WeakHandle.hpp>
#include <GL/glew.h>

#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::QueryPool
{
struct Impl
{
    Impl(const Device::Handle& a_Device, const uint32_t& a_Count, const std::vector<GLenum>& a_Targets, const Type& a_Type);
    ~Impl();
    virtual void Begin(const uint32_t& a_Index) {
        glBeginQuery(handles.at(a_Index), targets.at(a_Index));
    }
    virtual void End(const uint32_t& a_Index) {
        glEndQuery(targets.at(a_Index));
    }
    virtual void GetResult(
        const uint32_t& a_FirstQuery,
        const uint32_t& a_QueryCount,
        const size_t& a_DataSize,
        const void* a_Data,
        const size_t& a_Stride,
        const QueryResultFlags& a_Flags);
    const Type type;
    const Device::WeakHandle device;
    const std::vector<GLuint> handles;
    const std::vector<GLenum> targets;
};
struct PipelineStatistics : Impl
{
    PipelineStatistics(const Device::Handle& a_Device, const Info& a_Info);
    virtual void Begin(const uint32_t& a_Index) override {
        const auto handlesIndex = a_Index * pipelineStatisticsCount;
        for (auto targetIndex = 0; targetIndex < pipelineStatisticsCount; ++targetIndex) {
            Impl::Begin(handlesIndex + targetIndex);
        }
    }
    virtual void End(const uint32_t& a_Index) override {
        const auto handlesIndex = a_Index * pipelineStatisticsCount;
        for (auto targetIndex = 0; targetIndex < pipelineStatisticsCount; ++targetIndex) {
            Impl::End(handlesIndex + targetIndex);
        }
    }
    void GetResult(
        const uint32_t& a_FirstQuery,
        const uint32_t& a_QueryCount,
        const size_t& a_DataSize,
        const void* a_Data,
        const size_t& a_Stride,
        const QueryResultFlags& a_Flags) override;
    const uint32_t pipelineStatisticsCount;
};
}