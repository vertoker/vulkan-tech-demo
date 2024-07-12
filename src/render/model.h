#pragma once

#include "device.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

class VulkanModel {
public:
	struct Vertex {
		glm::vec2 position;

		static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
		static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
	};

	VulkanModel(VulkanDevice& device, const std::vector<Vertex>& vertices);
	~VulkanModel();

	VulkanModel(const VulkanModel&) = delete;
	VulkanModel& operator=(const VulkanModel&) = delete;

	void bind(VkCommandBuffer commandBuffer);
	void draw(VkCommandBuffer commandBuffer);

private:
	void createVertexBuffers(const std::vector<Vertex>& vertices);

	// Link to the device (can be deleted in the future)
	VulkanDevice& device;

	// Buffer on CPU, stored on RAM, edit here
	VkBuffer vertexBuffer;
	// Buffer on GPU, stored on GPU, copied from RAM on depends
	VkDeviceMemory vertexBufferMemory;
	// Counter for buffer
	uint32_t vertexCount;
};