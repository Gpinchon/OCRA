#pragma once

namespace OCRA
{
union ColorValue {
    float    float32[4]{ 0, 0, 0, 0 };
    int32_t  int32[4];
    uint32_t uint32[4];
};
}