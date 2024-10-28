#pragma once

#include "device.hpp"

// std
#include <memory>
#include <unordered_map>
#include <vector>

class VulkanDescriptorSetLayout {
    private:
    VulkanDevice& device;
    VkDescriptorSetLayout descriptorSetLayout;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};

    friend class VulkanDescriptorWriter;

    public:
    VulkanDescriptorSetLayout(VulkanDevice& device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
    ~VulkanDescriptorSetLayout();

    VulkanDescriptorSetLayout(const VulkanDescriptorSetLayout&) = delete;
    VulkanDescriptorSetLayout &operator=(const VulkanDescriptorSetLayout&) = delete;

    VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }
    
    class Builder {
        private:
        VulkanDevice& device;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};

        public:
        Builder(VulkanDevice& device) : device{device} {}

        Builder& addBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t count = 1);
        std::unique_ptr<VulkanDescriptorSetLayout> build() const;
    };
};

class VulkanDescriptorPool {
    private:
    VulkanDevice& device;
    VkDescriptorPool descriptorPool;

    friend class VulkanDescriptorWriter;

    public:
    VulkanDescriptorPool(VulkanDevice& device, uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize>& poolSizes);
    ~VulkanDescriptorPool();

    VulkanDescriptorPool(const VulkanDescriptorPool&) = delete;
    VulkanDescriptorPool &operator=(const VulkanDescriptorPool&) = delete;

    bool allocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;
    void freeDescriptor(std::vector<VkDescriptorSet>& descriptors) const;
    void resetPool();

    class Builder {
        private:
        VulkanDevice& device;
        std::vector<VkDescriptorPoolSize> poolSizes{};
        uint32_t maxSets = 1000;
        VkDescriptorPoolCreateFlags poolFlags = 0;

        public:
        Builder(VulkanDevice& device) : device{device} {}

        Builder &addPoolSize(VkDescriptorType descriptorType, uint32_t count);
        Builder &setPoolFlags(VkDescriptorPoolCreateFlags flags);
        Builder &setMaxSets(uint32_t count);
        std::unique_ptr<VulkanDescriptorPool> build() const;
    };
};

class VulkanDescriptorWriter {
    private:
    VulkanDescriptorSetLayout &setLayout;
    VulkanDescriptorPool &pool;
    std::vector<VkWriteDescriptorSet> writes;

    public:
    VulkanDescriptorWriter(VulkanDescriptorSetLayout &setLayout, VulkanDescriptorPool &pool);

    VulkanDescriptorWriter &writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
    VulkanDescriptorWriter &writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);

    bool build(VkDescriptorSet &set);
    void overwrite(VkDescriptorSet &set);
};