#include <OCRA/QueryPool.hpp>

#include <GL/Device.hpp>
#include <GL/QueryPool.hpp>
#include <GL/glew.h>

#include <stdexcept>

namespace OCRA::QueryPool
{
static inline auto GetQueryObject32(GLuint a_ID, GLenum a_Parameter)
{
    uint32_t result = 0;
    glGetQueryObjectuiv(a_ID, a_Parameter, &result);
    return result;
}
static inline auto GetQueryObject64(GLuint a_ID, GLenum a_Parameter)
{
    uint64_t result = 0;
    glGetQueryObjectui64v(a_ID, a_Parameter, &result);
    return result;
}
static inline auto GenQueries(const Device::Handle& a_Device, const uint32_t& a_Count)
{
    std::vector<GLuint> handles(a_Count);
    a_Device->PushCommand([data = handles.data(), a_Count]{
        glGenQueries(a_Count, data);
    }, true);
    return handles;
}
static inline auto GetPipelineQueryTargets(const PipelineStatisticFlags& a_Flags)
{
    std::vector<GLenum> targets;
    if ((a_Flags & PipelineStatisticFlagBits::InputAssemblyVertices) != 0)
        targets.push_back(GL_VERTICES_SUBMITTED);
    if ((a_Flags & PipelineStatisticFlagBits::InputAssemblyPrimitives) != 0)
        targets.push_back(GL_PRIMITIVES_SUBMITTED);
    if ((a_Flags & PipelineStatisticFlagBits::VertexShaderInvocations) != 0)
        targets.push_back(GL_VERTEX_SHADER_INVOCATIONS);
    if ((a_Flags & PipelineStatisticFlagBits::GeometryShaderInvocations) != 0)
        targets.push_back(GL_GEOMETRY_SHADER_INVOCATIONS);
    if ((a_Flags & PipelineStatisticFlagBits::GeometryShaderPrimitives) != 0)
        targets.push_back(GL_GEOMETRY_SHADER_PRIMITIVES_EMITTED);
    if ((a_Flags & PipelineStatisticFlagBits::ClippingInvocations) != 0)
        targets.push_back(GL_CLIPPING_INPUT_PRIMITIVES);
    if ((a_Flags & PipelineStatisticFlagBits::ClippingPrimitives) != 0)
        targets.push_back(GL_CLIPPING_OUTPUT_PRIMITIVES);
    if ((a_Flags & PipelineStatisticFlagBits::FragmentShaderInvocations) != 0)
        targets.push_back(GL_FRAGMENT_SHADER_INVOCATIONS);
    if ((a_Flags & PipelineStatisticFlagBits::TessellationControlShaderPatches) != 0)
        targets.push_back(GL_TESS_CONTROL_SHADER_PATCHES);
    if ((a_Flags & PipelineStatisticFlagBits::TessellationEvaluationShaderInvocations) != 0)
        targets.push_back(GL_TESS_EVALUATION_SHADER_INVOCATIONS);
    if ((a_Flags & PipelineStatisticFlagBits::ComputeShaderInvocations) != 0)
        targets.push_back(GL_COMPUTE_SHADER_INVOCATIONS);
    return targets;
}
Impl::Impl(const Device::Handle& a_Device, const uint32_t& a_Count, const std::vector<GLenum>& a_Targets, const Type& a_Type)
    : type(a_Type)
    , device(a_Device)
    , handles(GenQueries(a_Device, a_Count))
    , targets(a_Targets)
{}
Impl::~Impl()
{
    device.lock()->PushCommand([handles = handles] {
        for (const auto& handle : handles)
            glDeleteQueries(1, &handle);
    }, true);
}
void Impl::GetResult(
        const uint32_t& a_FirstQuery,
        const uint32_t& a_QueryCount,
        const size_t& a_DataSize,
        void* const a_Data,
        const size_t& a_Stride,
        const QueryResultFlags& a_Flags)
{
    device.lock()->PushCommand([
        this,
        &a_FirstQuery,
        &a_QueryCount,
        &a_DataSize,
        &a_Data,
        &a_Stride,
        &a_Flags] {
        auto use64bits = (a_Flags & QueryResultFlagBits::Result64Bits) != 0;
        auto withAvailability = (a_Flags & QueryResultFlagBits::WithAvailability) != 0;
        auto waitResult = (a_Flags & QueryResultFlagBits::Wait) != 0;
        for (auto index = a_FirstQuery; index < a_FirstQuery + a_QueryCount; ++index) {
            const auto& handle = handles.at(index);
            if (use64bits) static_cast<uint64_t*>(a_Data)[a_Stride * 1] = GetQueryObject64(handle, waitResult ? GL_QUERY_RESULT : GL_QUERY_RESULT_NO_WAIT);
            else static_cast<uint32_t*>(a_Data)[a_Stride * 1] = GetQueryObject32(handle, waitResult ? GL_QUERY_RESULT : GL_QUERY_RESULT_NO_WAIT);
            if (withAvailability)
            {
                if (use64bits) static_cast<uint64_t*>(a_Data)[a_Stride * 2] = GetQueryObject64(handle, GL_QUERY_RESULT_AVAILABLE);
                else static_cast<uint32_t*>(a_Data)[a_Stride * 2] = GetQueryObject32(handle, GL_QUERY_RESULT_AVAILABLE);
            }
        }
    }, true);
}
PipelineStatistics::PipelineStatistics(const Device::Handle& a_Device, const Info& a_Info)
    : Impl(a_Device, a_Info.count * a_Info.pipelineStatistics.count(), GetPipelineQueryTargets(a_Info.pipelineStatistics), Type::PipelineStatistics)
    , pipelineStatisticsCount(a_Info.pipelineStatistics.count())
{}
void PipelineStatistics::GetResult(
        const uint32_t& a_FirstQuery,
        const uint32_t& a_QueryCount,
        const size_t& a_DataSize,
        void* const a_Data,
        const size_t& a_Stride,
        const QueryResultFlags& a_Flags)
{
    device.lock()->PushCommand([
        this,
        &a_FirstQuery,
        &a_QueryCount,
        &a_DataSize,
        &a_Data,
        &a_Stride,
        &a_Flags] {
        auto use64bits = (a_Flags & QueryResultFlagBits::Result64Bits) != 0;
        auto withAvailability = (a_Flags & QueryResultFlagBits::WithAvailability) != 0;
        auto waitResult = (a_Flags & QueryResultFlagBits::Wait) != 0;
        for (auto index = a_FirstQuery; index < a_FirstQuery + a_QueryCount; ++index) {
            const auto realIndex = pipelineStatisticsCount * index;
            for (auto dataIndex = 0; dataIndex < pipelineStatisticsCount; ++dataIndex)
            {
                const auto& handle = handles.at(realIndex + dataIndex);
                if (use64bits) static_cast<uint64_t*>(a_Data)[a_Stride * dataIndex] = GetQueryObject64(handle, waitResult ? GL_QUERY_RESULT : GL_QUERY_RESULT_NO_WAIT);
                else static_cast<uint32_t*>(a_Data)[a_Stride * dataIndex] = GetQueryObject32(handle, waitResult ? GL_QUERY_RESULT : GL_QUERY_RESULT_NO_WAIT);
                if (withAvailability)
                {
                    if (use64bits) static_cast<uint64_t*>(a_Data)[a_Stride * pipelineStatisticsCount] |= GetQueryObject64(handle, GL_QUERY_RESULT_AVAILABLE);
                    else static_cast<uint32_t*>(a_Data)[a_Stride * pipelineStatisticsCount] |= GetQueryObject32(handle, GL_QUERY_RESULT_AVAILABLE);
                }
            }
            
        }
    }, true);
}
Handle Create(
    const Device::Handle&   a_Device,
    const Info&             a_Info)
{
    switch (a_Info.type) {
    case(Type::Occlusion) :
        return Handle(new Impl(a_Device, a_Info.count, std::vector<GLenum>(a_Info.count, GL_SAMPLES_PASSED), Type::Occlusion));
    case(Type::TimeStamp):
        return Handle(new Impl(a_Device, a_Info.count, std::vector<GLenum>(a_Info.count, GL_TIME_ELAPSED), Type::TimeStamp));
    case(Type::PipelineStatistics):
        return Handle(new PipelineStatistics(a_Device, a_Info));
    default:
        throw std::runtime_error("Unknown Barrier type");
    }
}
void GetResult(
    const Handle&   a_QueryPool,
    const uint32_t& a_FirstQuery,
    const uint32_t& a_QueryCount,
    const size_t&   a_DataSize,
    void* const     a_Data,
    const size_t    a_Stride,
    const QueryResultFlags& a_Flags)
{
    a_QueryPool->GetResult(
        a_FirstQuery,
        a_QueryCount,
        a_DataSize,
        a_Data,
        a_Stride,
        a_Flags);
}
}