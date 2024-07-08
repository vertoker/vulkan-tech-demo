#pragma once

#include <string>
#include <vector>

class VulkanPipeline {
public:
	VulkanPipeline(const std::string& vertFilePath, const std::string& fragFilePath);
private:
	static std::vector<char> readShader(const std::string& filePath);
};