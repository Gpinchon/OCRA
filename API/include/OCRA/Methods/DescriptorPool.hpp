#pragma once

#include <OCRA/Structs.hpp>

/**
* One muse always make sure Descriptor Pool outlives its Descriptor Sets
*/
namespace OCRA::Descriptor::Pool
{
/**
* Allocates a descriptor with the corresponding layout
*/
Descriptor::Set::Handle AllocateDescriptorSet(
    const Descriptor::Pool::Handle&  a_Pool,
    const AllocateDescriptorSetInfo& a_Info);
}