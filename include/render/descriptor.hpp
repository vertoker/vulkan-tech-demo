#pragma once

#include "device.hpp"

// std
#include <memory>
#include <unordered_map>
#include <vector>

class VulkanDescriptorSetLayout {
private:
    VulkanDevice& device;
    VkDescriptorSetLayout vkDescriptorSetLayout;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> vkBindings{};

    friend class VulkanDescriptorWriter;

public:
    VulkanDescriptorSetLayout(VulkanDevice& device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> vkBindings);
    ~VulkanDescriptorSetLayout();

    VulkanDescriptorSetLayout(const VulkanDescriptorSetLayout&) = delete;
    VulkanDescriptorSetLayout &operator=(const VulkanDescriptorSetLayout&) = delete;

    VkDescriptorSetLayout getDescriptorSetLayout() const { return vkDescriptorSetLayout; }
    
    class Builder {
    private:
        VulkanDevice& device;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> vkBindings{};

    public:
        Builder(VulkanDevice& device) : device{device} {}

        Builder& addBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t count = 1);
        std::unique_ptr<VulkanDescriptorSetLayout> build() const;
    };
};

class VulkanDescriptorPool {
private:
    VulkanDevice &device;
    VkDescriptorPool vkDescriptorPool;

    friend class VulkanDescriptorWriter;

public:
    VulkanDescriptorPool(VulkanDevice& device, uint32_t maxSets, VkDescriptorPoolCreateFlags vkPoolFlags, const std::vector<VkDescriptorPoolSize>& vkPoolSizes);
    ~VulkanDescriptorPool();

    VulkanDescriptorPool(const VulkanDescriptorPool&) = delete;
    VulkanDescriptorPool &operator=(const VulkanDescriptorPool&) = delete;

    bool tryAllocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;
    void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;
    void resetPool();

    VkDescriptorPool descriptorPool() { return vkDescriptorPool; }

    class Builder {
    private:
        VulkanDevice &device;
        std::vector<VkDescriptorPoolSize> vkPoolSizes{};
        uint32_t maxSets = 1000;
        VkDescriptorPoolCreateFlags vkPoolFlags = 0;

    public:
        Builder(VulkanDevice &device) : device{device} {}

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
    std::vector<VkWriteDescriptorSet> vkWrites;

public:
    VulkanDescriptorWriter(VulkanDescriptorSetLayout &setLayout, VulkanDescriptorPool &pool);

    VulkanDescriptorWriter &writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
    VulkanDescriptorWriter &writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);

    bool build(VkDescriptorSet &set);
    void overwrite(VkDescriptorSet &set);
};