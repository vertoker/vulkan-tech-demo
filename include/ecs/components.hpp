#pragma once

#include "render/model.hpp"

#include <fmt/core.h>

#include <memory>

struct Transform {
	glm::vec3 position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 rotation{ 0.0f, 0.0f, 0.0f };
	//glm::quat rotation{ 1.0f, 0.0f, 0.0f, 0.0f };
	glm::vec3 scale{ 1.0f, 1.0f, 1.0f };

	std::string position_str() const { return fmt::format("({}, {}, {})", position.x, position.y, position.z); }
	std::string rotation_str() const { return fmt::format("({}, {}, {})", rotation.x, rotation.y, rotation.z); }
	std::string scale_str() const { return fmt::format("({}, {}, {})", scale.x, scale.y, scale.z); }

	//glm::mat4 rotation_matrix() const;

	//glm::mat4 matrix2();
	glm::mat4 matrix() const;
	glm::mat3 normalMatrix() const;
};

struct PointLight {
	float lightIntensity = 1.0f;
	glm::vec3 color{ 1.0f };

	std::string lightIntensity_str() const { return fmt::format("{}", lightIntensity); }
	std::string color_str() const { return fmt::format("({}, {}, {})", color.x, color.y, color.z); }
};

struct ModelRenderer {
	std::shared_ptr<VulkanModel> model;
};