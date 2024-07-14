#pragma once

#include "device.h"

// std
#include <string>
#include <vector>

// Config for all configurable parts of GPU pipeline
struct PipelineConfigInfo {
	// Input pipeline stage config
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
	// Viewport for rendering
	VkPipelineViewportStateCreateInfo viewportInfo;
	// Rasteriazation pipeline stage config
	VkPipelineRasterizationStateCreateInfo rasterizationInfo;
	// MSAA technology config
	VkPipelineMultisampleStateCreateInfo multisampleInfo;
	// Color blending pipeline stage config
	VkPipelineColorBlendAttachmentState colorBlendAttachment;

	VkPipelineDepthStencilStateCreateInfo depthStencilInfo;

	std::vector<VkDynamicState> dynamicStateEnables;
	VkPipelineDynamicStateCreateInfo dynamicStateInfo;

	VkPipelineLayout pipelineLayout = nullptr;
	VkRenderPass renderPass = nullptr;
	uint32_t subpass = 0;
};

class VulkanPipeline {
public:
	VulkanPipeline(
		VulkanDevice& device, 
		const PipelineConfigInfo& configInfo,
		const std::string& vertFilePath,
		const std::string& fragFilePath);
	~VulkanPipeline();

	VulkanPipeline() = default;
	VulkanPipeline(const VulkanPipeline&) = delete;
	VulkanPipeline& operator=(const VulkanPipeline&) = delete;

	void bind(VkCommandBuffer commandBuffer);

	static void defaultConfigInfo(PipelineConfigInfo& configInfo);

private:
	VulkanDevice& device;

	VkPipeline graphicsPipeline;
	VkShaderModule vertShaderModule;
	VkShaderModule fragShaderModule;

	static std::vector<char> readShader(const std::string& filePath);

	void createGraphicsPipeline(
		const PipelineConfigInfo& configInfo,
		const std::string& vertFilePath, 
		const std::string& fragFilePath);
	void createShaderModule(
		const std::vector<char>& code, 
		VkShaderModule* shaderModule);
};