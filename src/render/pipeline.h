#pragma once

#include "device.h"

// std
#include <string>
#include <vector>

struct PipelineConfigInfo {

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