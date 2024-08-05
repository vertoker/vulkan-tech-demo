#include "model.h"

VulkanModel::VulkanModel(VulkanDevice& device, const std::vector<Vertex>& vertices)
	: device(device)
{
	createVertexBuffers(vertices);
}
VulkanModel::~VulkanModel()
{
	vkDestroyBuffer(device.device(), vertexBuffer, nullptr);
	vkFreeMemory(device.device(), vertexBufferMemory, nullptr);
}

void VulkanModel::createVertexBuffers(const std::vector<Vertex>& vertices)
{
	vertexCount = static_cast<uint32_t>(vertices.size());
	assert(vertexCount >= 3 && "Vertex count must be at least 3");
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
	// Host = CPU, Device = GPU
	// Create buffer, which connect CPU and GPU memory
	device.createBuffer(bufferSize, 
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		vertexBuffer, vertexBufferMemory
	);

	// Implementation is interleaved, or just in single buffer

	// Create abstract pointer
	void* data;
	// Bind pointer to the CPU buffer in Vulkan
	vkMapMemory(device.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
	// Copy vertices to the pointer, which already binded to the Vulkan
	// Memory will be transferred to the GPU on the next frame update (Flush())
	memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
	// Unbind pointer from CPU buffer in Vulkan
	vkUnmapMemory(device.device(), vertexBufferMemory);
}

void VulkanModel::bind(VkCommandBuffer commandBuffer)
{
	VkBuffer buffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}
void VulkanModel::draw(VkCommandBuffer commandBuffer)
{
	vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
}

std::vector<VkVertexInputBindingDescription> VulkanModel::Vertex::getBindingDescriptions()
{
	std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);

	bindingDescriptions[0].binding = 0;
	bindingDescriptions[0].stride = sizeof(Vertex);
	bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescriptions;
}
std::vector<VkVertexInputAttributeDescription> VulkanModel::Vertex::getAttributeDescriptions()
{
	std::vector<VkVertexInputAttributeDescription> attribureDescriptions(2);

	// For position
	attribureDescriptions[0].location = 0;
	attribureDescriptions[0].binding = 0;
	attribureDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attribureDescriptions[0].offset = offsetof(Vertex, position);

	// For color
	attribureDescriptions[1].location = 1;
	attribureDescriptions[1].binding = 0;
	attribureDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attribureDescriptions[1].offset = offsetof(Vertex, color);

	return attribureDescriptions;
}
