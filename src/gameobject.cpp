#include "gameobject.h"

glm::mat2 Transform2D::mat2()
{
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
