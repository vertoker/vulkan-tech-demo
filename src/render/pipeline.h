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
	VkViewport viewport;
	VkRect2D scissor;
	VkPipelineViewportStateCreateInfo viewportInfo;
	// Rasteriazation pipeline stage config
	VkPipelineRasterizationStateCreateInfo rasterizationInfo;
	// MSAA technology config
	VkPipelineMultisampleStateCreateInfo multisampleInfo;
	// Color blending pipeline stage config
	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	VkPipelineColorBlendStateCreateInfo colorBlendInfo;

	VkPipelineDepthStencilStateCreateInfo depthStencilInfo;

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

	VulkanPipeline(const VulkanPipeline&) = delete;
	void operator=(const VulkanPipeline&) = delete;

	static PipelineConfigInfo defaultConfigInfo(uint32_t width, uint32_t height);

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