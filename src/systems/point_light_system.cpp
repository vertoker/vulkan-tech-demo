#include "systems/point_light_system.hpp"

#include <glm/glm.hpp>
#include <fmt/core.h>

#include <ecs/components.hpp>
#include <glm/ext/matrix_transform.hpp>

struct PointLightPushConstants {
	glm::vec4 position{};
	glm::vec4 color{};
	float radius;
};

PointLightSystem::PointLightSystem(VulkanDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout,
	const std::string& vertFilePath, const std::string& fragFilePath) : VulkanWorldSystem(device)
{
	createPipelineLayout(globalSetLayout);
	createPipeline(renderPass, vertFilePath, fragFilePath);
}
PointLightSystem::~PointLightSystem()
{
	vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}

void PointLightSystem::updateLights(VulkanFrameInfo& frameInfo, UniformBufferObject& ubo)
{
	auto rotateLight = glm::rotate(glm::mat4(1.f), 0.5f * frameInfo.deltaTime, {0.f, -1.f, 0.f});
	int lightIndex = 0;

	auto view = frameInfo.registry.view<Transform, const PointLight>();
	for (auto&& [entity, transform, light] : view.each()) {
		//if (&light == NULL) continue;

		assert(lightIndex < MAX_LIGHTS && "Point lights exceed maximum specified");

		transform.position = glm::vec3(rotateLight * glm::vec4(transform.position, 1.f));

		ubo.pointLights[lightIndex].position = glm::vec4(transform.position, 1.0f);
		ubo.pointLights[lightIndex].color = glm::vec4(light.color, light.lightIntensity);

		lightIndex += 1;
	}
	ubo.numLights = lightIndex;
}

void PointLightSystem::render(VulkanFrameInfo& frameInfo)
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

	auto view = frameInfo.registry.view<Transform, PointLight>();
	for (auto&& [entity, transform, light] : view.each()) {
		//fmt::print("{} {} {} ; ", entity, transform.position_str(), light.color_str());
		//if (&light == NULL) continue;

		PointLightPushConstants push{}; // TODO fix this shit
		push.position = glm::vec4(transform.position, 1.0f);
		push.color = glm::vec4(light.color, light.lightIntensity);
		push.radius = transform.scale.x;

		vkCmdPushConstants(
			frameInfo.commandBuffer,
			pipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0,
			sizeof(PointLightPushConstants),
			&push
		);

		vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
	}
	//fmt::print("\n");
}

void PointLightSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
{
	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(PointLightPushConstants);

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
void PointLightSystem::createPipeline(VkRenderPass renderPass, 
	const std::string& vertFilePath, const std::string& fragFilePath)
{
	assert(pipelineLayout != nullptr && "Can't create pipeline before pipeline layout");

	PipelineConfigInfo pipelineInfo{};
	VulkanPipeline::defaultConfigInfo(pipelineInfo);
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.pipelineLayout = pipelineLayout;
	pipelineInfo.attributeDescription.clear();
	pipelineInfo.bindingDescription.clear();

	pipeline = std::make_unique<VulkanPipeline>(device, pipelineInfo, vertFilePath, fragFilePath);
}
