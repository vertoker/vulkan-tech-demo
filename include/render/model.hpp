#pragma once

#include "device.hpp"
#include "buffer.hpp"

// GLM
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <memory>
#include <vector>

class VulkanModel {
public:
	struct Vertex {
		glm::vec3 position{};
		glm::vec3 color{};
		glm::vec3 normal{};
		glm::vec2 uv{};

		static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
		static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

		bool operator==(const Vertex& other) const {
			return position == other.position
				&& color == other.color
				&& normal == other.normal
				&& uv == other.uv;
		}
	};

	struct Builder {
		std::vector<Vertex> vertices{};
		std::vector<uint32_t> indices{};

		void loadModel(const std::string& filepath);
	};

	VulkanModel(VulkanDevice& device, const Builder& builder);
	~VulkanModel();

	VulkanModel(const VulkanModel&) = delete;
	VulkanModel& operator=(const VulkanModel&) = delete;

	static std::shared_ptr<VulkanModel> createModelFromFile(VulkanDevice& device, const std::string& filepath);

	void bind(VkCommandBuffer commandBuffer);
	void draw(VkCommandBuffer commandBuffer);

private:
	void createVertexBuffers(const std::vector<Vertex>& vertices);
	void createIndexBuffers(const std::vector<uint32_t>& indices);

	// Link to the device (can be deleted in the future)
	VulkanDevice& device;

	std::unique_ptr<VulkanBuffer> vertexBuffer;
	uint32_t vertexCount;

	// for index buffer
	bool hasIndexBuffer = false;
	std::unique_ptr<VulkanBuffer> indexBuffer;
	uint32_t indexCount;
};