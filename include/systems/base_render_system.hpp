#pragma once

#include "render/device.hpp"
#include "render/pipeline.hpp"
#include "render/camera.hpp"
#include "render/frameInfo.hpp"

// Libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <iostream>
#include <memory>
#include <vector>

class VulkanRenderable {
public:
	virtual void render(VulkanFrameInfo& frameInfo) {}
	VulkanRenderable() = default;

	VulkanRenderable(const VulkanRenderable&) = delete;
	VulkanRenderable& operator=(const VulkanRenderable&) = delete;
};

class VulkanWorldSystem {
public:
	VulkanWorldSystem(VulkanDevice& device) : device{device} {}

protected:
	virtual void createPipelineLayout(VkDescriptorSetLayout globalSetLayout) {}
	virtual void createPipeline(VkRenderPass renderPass, 
		const std::string& vertFilePath, const std::string& fragFilePath) {}
	
	VulkanDevice& device;

	std::unique_ptr<VulkanPipeline> pipeline;
	std::string vertShaderPath, fragShaderPath;
	VkPipelineLayout pipelineLayout;
};
