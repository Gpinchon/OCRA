#pragma once

#include <Handle.hpp>

#include <bitset>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::QueryPool);

namespace OCRA::QueryPool
{
enum class Type {
    Unknown = -1,
    Occlusion,
    PipelineStatistics,
    TimeStamp
};
using PipelineStatisticFlags = std::bitset<11>;
namespace PipelineStatisticFlagBits {
static PipelineStatisticFlags None                                      = 0b00000000000;
static PipelineStatisticFlags InputAssemblyVertices                     = 0b10000000000;
static PipelineStatisticFlags InputAssemblyPrimitives                   = 0b01000000000;
static PipelineStatisticFlags VertexShaderInvocations                   = 0b00100000000;
static PipelineStatisticFlags GeometryShaderInvocations                 = 0b00010000000;
static PipelineStatisticFlags GeometryShaderPrimitives                  = 0b00001000000;
static PipelineStatisticFlags ClippingInvocations                       = 0b00000100000;
static PipelineStatisticFlags ClippingPrimitives                        = 0b00000010000;
static PipelineStatisticFlags FragmentShaderInvocations                 = 0b00000001000;
static PipelineStatisticFlags TessellationControlShaderPatches          = 0b00000000100;
static PipelineStatisticFlags TessellationEvaluationShaderInvocations   = 0b00000000010;
static PipelineStatisticFlags ComputeShaderInvocations                  = 0b00000000001;
}
using QueryResultFlags = std::bitset<4>;
namespace QueryResultFlagBits {
static QueryResultFlags None                = 0b0000;
static QueryResultFlags Result64Bits        = 0b1000; //result stored in 64 bits
static QueryResultFlags Wait                = 0b0100; //wait for the query result to be available
static QueryResultFlags WithAvailability    = 0b0010; //the availability status will accompany the results
static QueryResultFlags Partial             = 0b0001; //specifies that partial result is acceptable
}
struct Info {
    Type                    type{ Type::Unknown };
    uint32_t                count{ 0 };
    PipelineStatisticFlags  pipelineStatistics;
};
Handle Create(
    const Device::Handle&   a_Device,
    const Info&             a_Info);
void GetResult(
    const Handle&   a_QueryPool,
    const uint32_t& a_FirstQuery,
    const uint32_t& a_QueryCount,
    const size_t&   a_DataSize,
    const void*     a_Data,
    const size_t    a_Stride,
    const QueryResultFlags& a_Flags);
}