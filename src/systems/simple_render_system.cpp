#include "simple_render_system.h"

struct PushConstantData {
	glm::mat2 transform{ 1.f }; // Identity matrix
	glm::vec2 offset;
	// alignas is for memory specification for shader declaration
	alignas(16) glm::vec3 color;
};

SimpleRenderSystem::SimpleRenderSystem(VulkanDevice& device, VkRenderPass renderPass, 
	const std::string& vertFilePath,
	const std::string& fragFilePath)
	: device{device}
{
	createPipelineLayout();
	createPipeline(renderPass, vertFilePath, fragFilePath);
}
SimpleRenderSystem::~SimpleRenderSystem()
{
	vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}

void SimpleRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects)
{
	// Test rotation 2
	float i = 0;
	for (auto& obj : gameObjects) {
		i += 0.01f;
		obj.transform2D.rotation = glm::mod(obj.transform2D.rotation + 0.001f * i, 2.0f * glm::two_pi<float>());
	}

	pipeline->bind(commandBuffer);

	for (auto& obj : gameObjects) {
		// Test rotation anim
		//obj.transform2D.rotation = glm::mod(obj.transform2D.rotation + 0.01f, glm::two_pi<float>());

		PushConstantData push{};
		// Temp movement
		push.offset = obj.transform2D.position;
		push.color = obj.color;
		push.transform = obj.transform2D.mat2();

		vkCmdPushConstants(commandBuffer, pipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0, sizeof(PushConstantData), &push);
		obj.model->bind(commandBuffer);
		obj.model->draw(commandBuffer);
	}
}

void SimpleRenderSystem::createPipelineLayout()
{
	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(PushConstantData);

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

	if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		throw std::runtime_error("failed to create pipeline layout");
}
void SimpleRenderSystem::createPipeline(VkRenderPass renderPass, 
	const std::string& vertFilePath,
	const std::string& fragFilePath)
{
	assert(pipelineLayout != nullptr && "Can't create pipeline before pipeline layout");

	PipelineConfigInfo pipelineInfo{};
	VulkanPipeline::defaultConfigInfo(pipelineInfo);
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.pipelineLayout = pipelineLayout;

	pipeline = std::make_unique<VulkanPipeline>(device, pipelineInfo, vertFilePath, fragFilePath);
}
