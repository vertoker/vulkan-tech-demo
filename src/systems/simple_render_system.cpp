#include "simple_render_system.h"

struct PushConstantData {
	glm::mat4 transform{ 1.f }; // Identity matrix
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
	pipeline->bind(commandBuffer);

	for (auto& obj : gameObjects) {
		// Test rotation anim
		obj.transform.rotation.y = glm::mod(obj.transform.rotation.y + 0.01f, glm::two_pi<float>());
		obj.transform.rotation.x = glm::mod(obj.transform.rotation.x + 0.005f, glm::two_pi<float>());
		//obj.transform.rotation.w = glm::mod(obj.transform.rotation.w + 0.005f, 1.0f);

		PushConstantData push{};
		push.color = obj.color;
		push.transform = obj.transform.matrix2();

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
