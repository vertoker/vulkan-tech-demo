#include "render/pipeline.h"
#include "render/model.h"

// std
#include <cassert>
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
	vkDestroyShaderModule(device.device(), vertShaderModule, nullptr);
	vkDestroyShaderModule(device.device(), fragShaderModule, nullptr);
	vkDestroyPipeline(device.device(), graphicsPipeline, nullptr);
}

void VulkanPipeline::createGraphicsPipeline(
	const PipelineConfigInfo& configInfo,
	const std::string& vertFilePath,
	const std::string& fragFilePath)
{
	assert(configInfo.pipelineLayout != VK_NULL_HANDLE &&
		"Cannot create graphics pipeline: no pipelineLayout provided in configInfo");
	assert(configInfo.renderPass != VK_NULL_HANDLE &&
		"Cannot create graphics pipeline: no renderPass provided in configInfo");

	auto vertCode = readShader(vertFilePath);
	auto fragCode = readShader(fragFilePath);

	createShaderModule(vertCode, &vertShaderModule);
	createShaderModule(fragCode, &fragShaderModule);

	VkPipelineShaderStageCreateInfo shaderStages[2]{};

	shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStages[0].pNext = nullptr;
	shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	shaderStages[0].module = vertShaderModule;
	// name of function in vertex shader
	shaderStages[0].pName = "main";
	shaderStages[0].flags = 0;
	shaderStages[0].pSpecializationInfo = nullptr;

	shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStages[1].pNext = nullptr;
	shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	shaderStages[1].module = fragShaderModule;
	shaderStages[1].pName = "main";
	shaderStages[1].flags = 0;
	shaderStages[1].pSpecializationInfo = nullptr;

	auto attributeDescriptions = VulkanModel::Vertex::getAttributeDescriptions();
	auto bindingDescriptions = VulkanModel::Vertex::getBindingDescriptions();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
	vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;

	VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
	// Color Blending stage config
	colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendInfo.attachmentCount = 1;
	colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
	// Optional
	colorBlendInfo.logicOpEnable = VK_FALSE;
	colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
	colorBlendInfo.blendConstants[0] = 0.0f;
	colorBlendInfo.blendConstants[1] = 0.0f;
	colorBlendInfo.blendConstants[2] = 0.0f;
	colorBlendInfo.blendConstants[3] = 0.0f;

	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
	pipelineInfo.pViewportState = &configInfo.viewportInfo;
	pipelineInfo.pRasterizationState = &configInfo.rasterizationInfo;
	pipelineInfo.pMultisampleState = &configInfo.multisampleInfo;
	pipelineInfo.pColorBlendState = &colorBlendInfo;
	pipelineInfo.pDepthStencilState = &configInfo.depthStencilInfo;
	pipelineInfo.pDynamicState = &configInfo.dynamicStateInfo;

	pipelineInfo.layout = configInfo.pipelineLayout;
	pipelineInfo.renderPass = configInfo.renderPass;
	pipelineInfo.subpass = configInfo.subpass;

	pipelineInfo.basePipelineIndex = -1;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	if (vkCreateGraphicsPipelines(device.device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
		throw std::runtime_error("failed to create graphics pipeline");
}

void VulkanPipeline::bind(VkCommandBuffer commandBuffer)
{
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
}

void VulkanPipeline::defaultConfigInfo(PipelineConfigInfo& configInfo)
{
	// Converts vertices positions (-1/-1 to 1/1) to screen positions (0/0 to w/h)
	
	// Define, how Vulkan will be interpretent input vertex data
	configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	// Mostly it configure here, more about modes read in doc
	// TRIANGLE_LIST is just rendering triangles individually
	// TRIANGLE_STRIP is optimized method, which uses common vertices
	configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

	configInfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	configInfo.viewportInfo.viewportCount = 1;
	configInfo.viewportInfo.pViewports = nullptr;
	configInfo.viewportInfo.scissorCount = 1;
	configInfo.viewportInfo.pScissors = nullptr;
	
	// Settings for Vulkan of how it must convert vertex data to pixels in screen
	configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	// Clamp z coord of gl position clamp between 0 and 1
	configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
	configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
	// What rasterizer will do with triangles
	configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
	// Rasterizer line width
	configInfo.rasterizationInfo.lineWidth = 1.0f;
	// Mode for culling triangle (usually cull only back)
	configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	// What is Vulkan will be interpretent as front face (always set counter clockwise) (they inverted)
	// https://github.com/KhronosGroup/Vulkan-Docs/issues/320
	// VK_FRONT_FACE_COUNTER_CLOCKWISE VK_FRONT_FACE_CLOCKWISE
	configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	// Optional Depth Bias (blur for shadows or triangles)
	configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
	configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;
	configInfo.rasterizationInfo.depthBiasClamp = 0.0f;
	configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;

	// Settings for MSAA technology
	configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
	configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	// Optional
	configInfo.multisampleInfo.minSampleShading = 1.0f;
	configInfo.multisampleInfo.pSampleMask = nullptr;
	configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;
	configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;

	// Color Blending (part of color blending info)
	configInfo.colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	// Optional transparency
	configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
	configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
	configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
	configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
	// Optional
	configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
	configInfo.depthStencilInfo.minDepthBounds = 0.0f;
	configInfo.depthStencilInfo.maxDepthBounds = 1.0f;
	configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
	configInfo.depthStencilInfo.front = {};
	configInfo.depthStencilInfo.back = {};

	configInfo.dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
	configInfo.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	configInfo.dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(configInfo.dynamicStateEnables.size());
	configInfo.dynamicStateInfo.pDynamicStates = configInfo.dynamicStateEnables.data();
	configInfo.dynamicStateInfo.flags = 0;
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
