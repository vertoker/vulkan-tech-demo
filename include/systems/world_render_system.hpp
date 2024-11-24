#pragma once

#include "base_system.hpp"

class WorldRenderSystem : public BaseSystem, public BaseWorldRenderSystem {
public:
	WorldRenderSystem(VulkanDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout,
		const std::string& vertFilePath, const std::string& fragFilePath);
	~WorldRenderSystem();
	
	void execute(VulkanFrameInfo& frameInfo) override;

private:
	void createPipelineLayout(VkDescriptorSetLayout globalSetLayout) override;
	void createPipeline(VkRenderPass renderPass, 
		const std::string& vertFilePath, const std::string& fragFilePath) override;
};
