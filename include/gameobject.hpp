#pragma once

#include "render/model.hpp"
#include <glm/gtc/matrix_transform.hpp>

// std
#include <memory>
#include <format>
#include <unordered_map>

struct TransformComponent {
	glm::vec3 position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 rotation{ 0.0f, 0.0f, 0.0f };
	//glm::quat rotation{ 1.0f, 0.0f, 0.0f, 0.0f };
	glm::vec3 scale{ 1.f, 1.f, 1.f };

	std::string position_str() { return std::format("({}, {}, {})", position.x, position.y, position.z); }
	std::string rotation_str() { return std::format("({}, {}, {})", rotation.x, rotation.y, rotation.z); }
	std::string scale_str() { return std::format("({}, {}, {})", scale.x, scale.y, scale.z); }

	//glm::mat4 rotation_matrix() const;

	//glm::mat4 matrix2();
	glm::mat4 matrix();
	glm::mat3 normalMatrix();
};

struct PointLightComponent {
	float lightIntensity = 1.0f;
};

// Unity like
class GameObject {
public:
	using id_t = unsigned int;
	using map = std::unordered_map<id_t, GameObject>;

	static GameObject createGameObject() {
		static id_t currentId = 0;
		return GameObject{ currentId++ };
	}
	static GameObject createPointLight(float intensity, float radius, glm::vec3 color);

	const id_t getId() { return id; }

	// TODO convert all of this into ECS

	glm::vec3 color{};
	TransformComponent transform{};

	// Optional
	std::shared_ptr<VulkanModel> model{};
	std::unique_ptr<PointLightComponent> pointLight = nullptr;

private:
	GameObject(id_t objId) : id{ objId } {}

	id_t id;
};