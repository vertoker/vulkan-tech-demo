#pragma once

#include "base_system.hpp"

class ImguiSystem : public BaseSystem {
public:
	ImguiSystem(VulkanDevice& device, VkRenderPass renderPass, VkDescriptorPool descriptorPool, GLFWwindow* window);
	~ImguiSystem();

	void execute(VulkanFrameInfo& frameInfo) override;
};