#pragma once

#include "render/model.h"

// std
#include <memory>

struct Transform2D {
	glm::vec2 position;
	glm::vec2 scale{ 1.f, 1.f };
	float rotation;

	glm::mat2 mat2() {
		const float s = glm::sin(rotation);
		const float c = glm::cos(rotation);
		glm::mat2 rotationMatrix{ {c, s},{-s, c} };

		// Identity matrix, but with scaling
		glm::mat2 scaleMatrix{ {scale.x, 0.0f},{0.0f, scale.y} };

		// Matrix multiplication is not commuticative
		// and for correct result, use next order:
		// rotation -> scale -> position
		return rotationMatrix * scaleMatrix;
	}
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

private:
	GameObject(id_t objId) : id{ objId } {}

	id_t id;
};