#pragma once

#include <OCRA/Descriptor/Set.hpp>
#include <OCRA/Descriptor/Pool.hpp>
#include <OCRA/Descriptor/SetLayout.hpp>

#include <OCRA/Handle.hpp>

OCRA_DECLARE_HANDLE(OCRA::Descriptor::Pool);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::Set);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::SetLayout);

namespace OCRA {
class Uniform {
public:
    Uniform() = default;
    Uniform(const Uniform&) = default;
    //We already know how our uniforms are formated
    Uniform(const Device::Handle& a_Device, const std::vector<Descriptor::SetLayout::Binding>& a_Bindings)
        : device(a_Device)
        , descriptorSetLayoutBindings(a_Bindings)
    {}
    //We don't know our bindings yet and will call CreateDescriptorSetLayout & CreateDescriptorSets later
    Uniform(const Device::Handle& a_Device)
        : device(a_Device)
    {}
    auto& GetDevice() const { return device; };
    auto& GetDescriptorSetLayoutBindings() const { return descriptorSetLayoutBindings; }
    virtual const std::vector<Descriptor::Set::WriteOperation>& GetWriteOperations() const final {
        return writeOperations;
    }

protected:
    //To be called when the data needs updating
    virtual void Update() = 0;
    void SetDescriptorSetLayoutBindings(const std::vector<Descriptor::SetLayout::Binding>& a_Bindings) {
        descriptorSetLayoutBindings = a_Bindings;
    }
    void SetWriteOperations(const std::vector<Descriptor::Set::WriteOperation>& a_WriteOperations) {
        writeOperations = a_WriteOperations;
    }

private:
    const Device::Handle                         device;
    std::vector<Descriptor::SetLayout::Binding>  descriptorSetLayoutBindings;
    std::vector<Descriptor::Set::WriteOperation> writeOperations;
};
}