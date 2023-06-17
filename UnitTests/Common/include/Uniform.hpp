#pragma once

#include <OCRA/OCRA.hpp>

#include <OCRA/Handle.hpp>

namespace OCRA {
class Uniform {
public:
    Uniform()               = default;
    Uniform(const Uniform&) = default;
    // We already know how our uniforms are formated
    Uniform(const Device::Handle& a_Device, const std::vector<DescriptorSetBinding>& a_Bindings)
        : device(a_Device)
        , descriptorSetBindings(a_Bindings)
    {
    }
    // We don't know our bindings yet and will call CreateDescriptorSetLayout & CreateDescriptorSets later
    Uniform(const Device::Handle& a_Device)
        : device(a_Device)
    {
    }
    auto& GetDevice() const { return device; };
    auto& GetDescriptorSetLayoutBindings() const { return descriptorSetBindings; }
    virtual const std::vector<DescriptorSetWrite>& GetWriteOperations() const final
    {
        return writeOperations;
    }

protected:
    /**
     * @brief To be called when the data needs updating
     * @return true if an update took place
     */
    virtual bool Update() = 0;
    void SetDescriptorSetBindings(const std::vector<DescriptorSetBinding>& a_Bindings)
    {
        descriptorSetBindings = a_Bindings;
    }
    void SetWriteOperations(const std::vector<DescriptorSetWrite>& a_WriteOperations)
    {
        writeOperations = a_WriteOperations;
    }

private:
    const Device::Handle device;
    std::vector<DescriptorSetBinding> descriptorSetBindings;
    std::vector<DescriptorSetWrite> writeOperations;
};
}
