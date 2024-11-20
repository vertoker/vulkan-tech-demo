#pragma once

#include "render/model.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <format>

struct Transform {
	glm::vec3 position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 rotation{ 0.0f, 0.0f, 0.0f };
	//glm::quat rotation{ 1.0f, 0.0f, 0.0f, 0.0f };
	glm::vec3 scale{ 1.f, 1.f, 1.f };

	std::string position_str() { return std::format("({}, {}, {})", position.x, position.y, position.z); }
	std::string rotation_str() { return std::format("({}, {}, {})", rotation.x, rotation.y, rotation.z); }
	std::string scale_str() { return std::format("({}, {}, {})", scale.x, scale.y, scale.z); }

	//glm::mat4 rotation_matrix() const;

	//glm::mat4 matrix2();
	glm::mat4 matrix() const;
	glm::mat3 normalMatrix() const;
};

struct PointLight {
	float lightIntensity = 1.0f;
	glm::vec3 color{ 1.0f };
};

struct ModelRenderer {
	std::shared_ptr<VulkanModel> model;
};