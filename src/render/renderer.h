#pragma once

#include "window.h"
#include "swapchain.h"

// Libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <iostream>
#include <memory>
#include <vector>

class VulkanRenderer {
public:
	VulkanRenderer(VulkanWindow& window, VulkanDevice& device);
	~VulkanRenderer();

	VulkanRenderer(const VulkanRenderer&) = delete;
	VulkanRenderer& operator=(const VulkanRenderer&) = delete;
	
	VkRenderPass getSwapChainRenderPass() const { return swapChain->getRenderPass(); }
	bool isFrameInProgress() const { return isFrameStarted; }
	VkCommandBuffer getCurrentCommandBuffer() const 
	{
		assert(isFrameStarted && "Cannot get command buffer when frame is not in progress");
		return commandBuffers[currentFrameIndex];
	}

	int getFrameIndex() const {
		assert(isFrameStarted && "Cannot get command buffer when frame is not in progress");
		return currentFrameIndex;
	}

	VkCommandBuffer beginFrame();
	void endFrame();
	// Using different render passes, possible render other features: shadows or post processing
	void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
	void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

private:
	void createCommandBuffers();
	void freeCommandBuffers();
	void recreateSwapChain();

	VulkanWindow& window;
	VulkanDevice& device;
	std::unique_ptr<VulkanSwapChain> swapChain;
	std::vector<VkCommandBuffer> commandBuffers;

	uint32_t currentImageIndex;
	int currentFrameIndex = 0;
	bool isFrameStarted = false;
};
