#include "systems/world_render_system.hpp"

#include <ecs/components.hpp>

struct PushConstantData {
	glm::mat4 modelMatrix{ 1.f };
	glm::mat4 normalMatrix{ 1.f };
};

WorldRenderSystem::WorldRenderSystem(VulkanDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout,
	const std::string& vertFilePath, const std::string& fragFilePath) : BaseWorldRenderSystem(device)
{
	createPipelineLayout(globalSetLayout);
	createPipeline(renderPass, vertFilePath, fragFilePath);
}
WorldRenderSystem::~WorldRenderSystem()
{
	vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}

void WorldRenderSystem::execute(VulkanFrameInfo& frameInfo)
{
	pipeline->bind(frameInfo.commandBuffer);

	vkCmdBindDescriptorSets(
		frameInfo.commandBuffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		pipelineLayout,
		0, 1,
		&frameInfo.globalDescriptorSet,
		0, nullptr
	);

	auto view = frameInfo.registry.view<const Transform, const ModelRenderer>();
	for (auto&& [entity, transform, renderer] : view.each()) {
		//if (&renderer == NULL) continue;

		PushConstantData push{};
		push.modelMatrix = transform.matrix();
		push.normalMatrix = transform.normalMatrix();

		vkCmdPushConstants(frameInfo.commandBuffer, pipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0, sizeof(PushConstantData), &push);
		renderer.model->bind(frameInfo.commandBuffer);
		renderer.model->draw(frameInfo.commandBuffer);
	}
}

void WorldRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
{
	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(PushConstantData);

	std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
	pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

	if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		throw std::runtime_error("failed to create pipeline layout");
}
void WorldRenderSystem::createPipeline(VkRenderPass renderPass, 
	const std::string& vertFilePath, const std::string& fragFilePath)
{
	assert(pipelineLayout != nullptr && "Can't create pipeline before pipeline layout");

	PipelineConfigInfo pipelineInfo{};
	VulkanPipeline::defaultConfigInfo(pipelineInfo);
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.pipelineLayout = pipelineLayout;

	pipeline = std::make_unique<VulkanPipeline>(device, pipelineInfo, vertFilePath, fragFilePath);
}
