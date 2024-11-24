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

class BaseSystem {
public:
	virtual void execute(VulkanFrameInfo& frameInfo) {}
	BaseSystem() = default;

	BaseSystem(const BaseSystem&) = delete;
	BaseSystem& operator=(const BaseSystem&) = delete;
};

class BaseWorldRenderSystem {
public:
	BaseWorldRenderSystem(VulkanDevice& device) : device{device} {}

protected:
	virtual void createPipelineLayout(VkDescriptorSetLayout globalSetLayout) {}
	virtual void createPipeline(VkRenderPass renderPass, 
		const std::string& vertFilePath, const std::string& fragFilePath) {}
	
	VulkanDevice& device;

	std::unique_ptr<VulkanPipeline> pipeline;
	std::string vertShaderPath, fragShaderPath;
	VkPipelineLayout pipelineLayout;
};
