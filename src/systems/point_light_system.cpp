#include "systems/point_light_system.hpp"
#include "glm/glm.hpp"

struct PointLightPushConstants {
	glm::vec4 position{};
	glm::vec4 color{};
	float radius;
};

PointLightSystem::PointLightSystem(VulkanDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout,
	const std::string& vertFilePath, const std::string& fragFilePath) : VulkanRenderSystem(device)
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

	for (auto& kv : frameInfo.gameObjects) {
		auto& obj = kv.second;
		if (obj.pointLight == nullptr) continue;

		assert(lightIndex < MAX_LIGHTS && "Point lights exceed maximum specified");

		obj.transform.position = glm::vec3(rotateLight * glm::vec4(obj.transform.position, 1.f));

		ubo.pointLights[lightIndex].position = glm::vec4(obj.transform.position, 1.0f);
		ubo.pointLights[lightIndex].color = glm::vec4(obj.color, obj.pointLight->lightIntensity);

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

	for (auto& kv : frameInfo.gameObjects) {
		auto& obj = kv.second;
		if (obj.pointLight == nullptr) continue;

		PointLightPushConstants push{}; // TODO fix this shit
		push.position = glm::vec4(obj.transform.position, 1.0f);
		push.color = glm::vec4(obj.color, obj.pointLight->lightIntensity);
		push.radius = obj.transform.scale.x;

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
