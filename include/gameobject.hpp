#pragma once

#include "render/model.hpp"
#include <glm/gtc/matrix_transform.hpp>

// std
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
	glm::mat4 matrix();
	glm::mat3 normalMatrix();
};

// Unity like
class GameObject {
public:
	using id_t = unsigned int;

	static GameObject createGameObject() {
		static id_t currentId = 0;
		return GameObject{ currentId++ };
	}

	const id_t getId() { return id; }

	std::shared_ptr<VulkanModel> model{};
	glm::vec3 color{};
	Transform transform{};

private:
	GameObject(id_t objId) : id{ objId } {}

	id_t id;
};