#include "pipeline.h"

// std
#include <iostream>
#include <fstream>

VulkanPipeline::VulkanPipeline(const std::string& vertFilePath, const std::string& fragFilePath)
{
	auto vertCode = readShader(vertFilePath);
	auto fragCode = readShader(fragFilePath);

	std::cout << "Vertex shader code size: " << vertCode.size() << std::endl;
	std::cout << "Fragment shader code size: " << fragCode.size() << std::endl;
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
