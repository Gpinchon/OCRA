#pragma once

#include <OCRA/Structs.hpp>

#include <vector>

namespace OCRA::Descriptor::Set
{
void Update(
    const std::vector<DescriptorSetWrite>&  a_Writes,
    const std::vector<DescriptorSetCopy>& a_Copies);
}
