#pragma once

#include "render_system.hpp"

class PointLightSystem : public VulkanRenderSystem {
public:
	PointLightSystem(VulkanDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout,
		const std::string& vertFilePath, const std::string& fragFilePath);
	~PointLightSystem() override;
	
	void updateLights(VulkanFrameInfo& frameInfo, UniformBufferObject& ubo);
	void render(VulkanFrameInfo& frameInfo) override;

private:
	void createPipelineLayout(VkDescriptorSetLayout globalSetLayout) override;
	void createPipeline(VkRenderPass renderPass, 
		const std::string& vertFilePath, const std::string& fragFilePath) override;
};
