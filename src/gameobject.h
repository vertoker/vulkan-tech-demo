#pragma once

#include "render/model.h"

// std
#include <memory>

struct Rigidbody2D {
	glm::vec2 velocity;
	float mass{ 1.0f };
};

struct Transform2D {
	glm::vec2 position;
	glm::vec2 scale{ 1.f, 1.f };
	float rotation;

	glm::mat2 mat2();
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
	Transform2D transform2D{};
	Rigidbody2D rigidbody2D{};

private:
	GameObject(id_t objId) : id{ objId } {}

	id_t id;
};