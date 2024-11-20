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

class VulkanRenderSystem {
public:
	VulkanRenderSystem(VulkanDevice& device) : device{device} {}
	virtual ~VulkanRenderSystem() {}

	VulkanRenderSystem(const VulkanRenderSystem&) = delete;
	VulkanRenderSystem& operator=(const VulkanRenderSystem&) = delete;

	virtual void render(VulkanFrameInfo& frameInfo) {}

protected:
	virtual void createPipelineLayout(VkDescriptorSetLayout globalSetLayout) {}
	virtual void createPipeline(VkRenderPass renderPass, 
		const std::string& vertFilePath, const std::string& fragFilePath) {}
	
	VulkanDevice& device;

	std::unique_ptr<VulkanPipeline> pipeline;
	std::string vertShaderPath, fragShaderPath;
	VkPipelineLayout pipelineLayout;
};
