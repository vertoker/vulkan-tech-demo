#include "pipeline.h"

// std
#include <iostream>
#include <fstream>
#include <stdexcept>

VulkanPipeline::VulkanPipeline(
	VulkanDevice& device, 
	const PipelineConfigInfo& configInfo, 
	const std::string& vertFilePath, 
	const std::string& fragFilePath)
	: device(device)
{
	createGraphicsPipeline(configInfo, vertFilePath, fragFilePath);
}
VulkanPipeline::~VulkanPipeline()
{

}

void VulkanPipeline::createGraphicsPipeline(
	const PipelineConfigInfo& configInfo,
	const std::string& vertFilePath,
	const std::string& fragFilePath)
{
	auto vertCode = readShader(vertFilePath);
	auto fragCode = readShader(fragFilePath);

	std::cout << "Vertex shader code size: " << vertCode.size() << std::endl;
	std::cout << "Fragment shader code size: " << fragCode.size() << std::endl;


}

PipelineConfigInfo VulkanPipeline::defaultConfigInfo(uint32_t width, uint32_t height)
{
	PipelineConfigInfo configInfo{};

	return PipelineConfigInfo();
}

std::vector<char> VulkanPipeline::readShader(const std::string& filePath)
{
	std::ifstream file{ filePath, std::ios::ate | std::ios::binary };
	
	if (!file.is_open()) {
		throw std::runtime_error("failed to open file: " + filePath);
	}
	
	auto fileSize = static_cast<size_t>(file.tellg());
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}

void VulkanPipeline::createShaderModule(
	const std::vector<char>& code, 
	VkShaderModule* shaderModule)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	if (vkCreateShaderModule(device.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module");
	}
}
