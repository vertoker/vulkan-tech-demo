#pragma once

#include "base_system.hpp"

class PointLightSystem : public BaseSystem, public BaseWorldRenderSystem {
public:
	PointLightSystem(VulkanDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout,
		const std::string& vertFilePath, const std::string& fragFilePath);
	~PointLightSystem();
	
	void updateLights(VulkanFrameInfo& frameInfo, UniformBufferObject& ubo);
	void execute(VulkanFrameInfo& frameInfo) override;

private:
	void createPipelineLayout(VkDescriptorSetLayout globalSetLayout) override;
	void createPipeline(VkRenderPass renderPass, 
		const std::string& vertFilePath, const std::string& fragFilePath) override;
};
