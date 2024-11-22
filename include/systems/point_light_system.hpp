#pragma once

#include "base_render_system.hpp"

class PointLightSystem : public VulkanRenderable, public VulkanWorldSystem {
public:
	PointLightSystem(VulkanDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout,
		const std::string& vertFilePath, const std::string& fragFilePath);
	~PointLightSystem();
	
	void updateLights(VulkanFrameInfo& frameInfo, UniformBufferObject& ubo);
	void render(VulkanFrameInfo& frameInfo) override;

private:
	void createPipelineLayout(VkDescriptorSetLayout globalSetLayout) override;
	void createPipeline(VkRenderPass renderPass, 
		const std::string& vertFilePath, const std::string& fragFilePath) override;
};
