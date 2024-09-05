#include "model.h"
#include "utils.h"

// libs
#define TINYOBJLOADER_IMPLEMENTATION // part of impl for this loader
#include <tiny_obj_loader.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

// std
#include <cassert>
#include <cstring>
#include <iostream>
#include <unordered_map>

namespace std {
	template <>
	struct hash<VulkanModel::Vertex> {
		size_t operator()(VulkanModel::Vertex const& vertex) const {
			size_t seed = 0;
			hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
			return seed;
		}
	};
}

VulkanModel::VulkanModel(VulkanDevice& device, const Builder& builder) : device(device)
{
	createVertexBuffers(builder.vertices);
	createIndexBuffers(builder.indices);
}
VulkanModel::~VulkanModel()
{
	vkDestroyBuffer(device.device(), vertexBuffer, nullptr);
	vkFreeMemory(device.device(), vertexBufferMemory, nullptr);

	if (hasIndexBuffer) {
		vkDestroyBuffer(device.device(), indexBuffer, nullptr);
		vkFreeMemory(device.device(), indexBufferMemory, nullptr);
	}
}

void VulkanModel::createVertexBuffers(const std::vector<Vertex>& vertices)
{
	vertexCount = static_cast<uint32_t>(vertices.size());
	VkDeviceSize bufferSize = sizeof(Vertex) * vertexCount;
	assert(vertexCount >= 3 && "Vertex count must be at least 3");

	// Create staging buffer
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	device.createBuffer(bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT, // for staging buffer (src)
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | // visible for the host
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, // can be updated by the host side
		stagingBuffer, stagingBufferMemory
	);

	// Implementation is interleaved, or just in single buffer

	// Create abstract pointer
	void* data;
	// Bind pointer to the CPU buffer in Vulkan, which connects CPU and GPU buffers
	vkMapMemory(device.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
	// Copy vertices to the pointer, which already binded to the Vulkan
	// Memory will be transferred to the GPU on the next frame update (Flush())
	memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
	// Unbind pointer from CPU buffer in Vulkan
	vkUnmapMemory(device.device(), stagingBufferMemory);

	// Host = CPU, Device = GPU
	// Create buffer on GPU (Device)
	device.createBuffer(bufferSize,
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | // vertex buffer
		VK_BUFFER_USAGE_TRANSFER_DST_BIT, // target for staging buffer
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, // internal for device, but faster
		// data will be taken from staging buffer
		vertexBuffer, vertexBufferMemory
	);

	// copy operation for staging buffer
	device.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

	// destroy staging buffer
	vkDestroyBuffer(device.device(), stagingBuffer, nullptr);
	vkFreeMemory(device.device(), stagingBufferMemory, nullptr);
}

void VulkanModel::createIndexBuffers(const std::vector<uint32_t>& indices)
{
	indexCount = static_cast<uint32_t>(indices.size());
	hasIndexBuffer = indexCount > 0;
	if (!hasIndexBuffer) { return; }

	VkDeviceSize bufferSize = sizeof(uint32_t) * indexCount;

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	device.createBuffer(bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer, stagingBufferMemory
	);

	void* data;
	vkMapMemory(device.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
	vkUnmapMemory(device.device(), stagingBufferMemory);

	device.createBuffer(bufferSize,
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT | // index buffer
		VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		indexBuffer, indexBufferMemory
	);

	device.copyBuffer(stagingBuffer, indexBuffer, bufferSize);

	vkDestroyBuffer(device.device(), stagingBuffer, nullptr);
	vkFreeMemory(device.device(), stagingBufferMemory, nullptr);
}

void VulkanModel::bind(VkCommandBuffer commandBuffer)
{
	VkBuffer buffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

	if (hasIndexBuffer) // same as buffer value size - uint32
		vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
	// uint16_t -> 2^16 - 1 = 65,535 vertices
	// uint32_t -> 2^32 - 1 = 4,294,967,295 vertices
}
void VulkanModel::draw(VkCommandBuffer commandBuffer)
{
	if (hasIndexBuffer)
		vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
	else vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
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
	std::vector<VkVertexInputAttributeDescription> attribureDescriptions{};

	attribureDescriptions.push_back({ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position) });
	attribureDescriptions.push_back({ 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color) });
	attribureDescriptions.push_back({ 2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal) });
	attribureDescriptions.push_back({ 3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv) });

	return attribureDescriptions;
}

std::unique_ptr<VulkanModel> VulkanModel::createModelFromFile(VulkanDevice& device, const std::string& filepath)
{
	Builder builder{};
	builder.loadModel(filepath);
	std::cout << "Loaded model, vertex count: " << builder.vertices.size() << 
		", indices count: " << builder.indices.size() << std::endl;
	return std::make_unique<VulkanModel>(device, builder);
}

void VulkanModel::Builder::loadModel(const std::string& filepath)
{
	tinyobj::attrib_t attrib; // pos/color/normal/texture data
	std::vector<tinyobj::shape_t> shapes; // indices for each face element
	std::vector<tinyobj::material_t> materials; // materials data (later)
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str()))
		throw std::runtime_error(warn + err);

	vertices.clear();
	indices.clear();

	// this for building own indices, using model
	std::unordered_map<Vertex, uint32_t> uniqueVertices{};

	for (const auto &shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex{};

			// if below 0 - no indices is provided
			if (index.vertex_index >= 0) {
				// data stores as float array, not vec3 array
				vertex.position = { 
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2],
				};

				vertex.color = {
					attrib.colors[3 * index.vertex_index + 0],
					attrib.colors[3 * index.vertex_index + 1],
					attrib.colors[3 * index.vertex_index + 2],
				};
			}

			// same but for normals
			if (index.normal_index >= 0) {
				vertex.normal = { 
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2],
				};
			}

			// same but for textures
			if (index.texcoord_index >= 0) {
				vertex.uv = { 
					attrib.texcoords[2 * index.texcoord_index + 0],
					attrib.texcoords[2 * index.texcoord_index + 1],
				};
			}

			if (!uniqueVertices.contains(vertex)) {
				// push unique index of vertex if no founded
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				// and also into vertices vector
				vertices.push_back(vertex);
			}
			// and push index of unique vertex for indices vertor
			indices.push_back(uniqueVertices[vertex]);
		}
	}
}
