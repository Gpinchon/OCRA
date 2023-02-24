#pragma once

#include <OCRA/Structs.hpp>

namespace OCRA::QueryPool
{
void GetResult(
    const Handle&   a_QueryPool,
    const uint32_t& a_FirstQuery,
    const uint32_t& a_QueryCount,
    const size_t&   a_DataSize,
    const void*     a_Data,
    const size_t    a_Stride,
    const QueryResultFlags& a_Flags);
}