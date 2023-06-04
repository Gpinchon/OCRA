#pragma once

#include <OCRA/OCRA.hpp>

#include <OCRA/Handle.hpp>

namespace OCRA {
class Uniform {
public:
    Uniform()               = default;
    Uniform(const Uniform&) = default;
    // We already know how our uniforms are formated
    Uniform(const Device::Handle& a_Device, const std::vector<DescriptorSetLayoutBinding>& a_Bindings)
        : device(a_Device)
        , descriptorSetLayoutBindings(a_Bindings)
    {
    }
    // We don't know our bindings yet and will call CreateDescriptorSetLayout & CreateDescriptorSets later
    Uniform(const Device::Handle& a_Device)
        : device(a_Device)
    {
    }
    auto& GetDevice() const { return device; };
    auto& GetDescriptorSetLayoutBindings() const { return descriptorSetLayoutBindings; }
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
    void SetDescriptorSetLayoutBindings(const std::vector<DescriptorSetLayoutBinding>& a_Bindings)
    {
        descriptorSetLayoutBindings = a_Bindings;
    }
    void SetWriteOperations(const std::vector<DescriptorSetWrite>& a_WriteOperations)
    {
        writeOperations = a_WriteOperations;
    }

private:
    const Device::Handle device;
    std::vector<DescriptorSetLayoutBinding> descriptorSetLayoutBindings;
    std::vector<DescriptorSetWrite> writeOperations;
};
}
