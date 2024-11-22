#pragma once

#include "base_render_system.hpp"

class ImguiSystem : public VulkanRenderable {
public:
	ImguiSystem(VulkanDevice& device, VkRenderPass renderPass, VkDescriptorPool descriptorPool, GLFWwindow* window);
	~ImguiSystem();

	void render(VulkanFrameInfo& frameInfo) override;
};