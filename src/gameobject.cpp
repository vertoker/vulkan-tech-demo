#include "gameobject.h"

// glm::translate
glm::mat4 Transform::position_matrix()
{
	glm::mat4 matrix
	{
		1.0f, 0.0f, 0.0f, position.x,
		0.0f, 1.0f, 0.0f, position.y,
		0.0f, 0.0f, 1.0f, position.z,
		0.0f, 0.0f, 0.0f,       1.0f
	};
	return matrix;
}

glm::mat4 Transform::rotation_matrix() const
{
	float x = rotation.x * 2.0f;
	float y = rotation.y * 2.0f;
	float z = rotation.z * 2.0f;
	float w = rotation.w * 2.0f;

	float xx = rotation.x * x;
	float yy = rotation.y * y;
	float zz = rotation.z * z;

	float xy = rotation.x * y;
	float xz = rotation.x * z;
	float yz = rotation.y * z;

	float wx = rotation.w * x;
	float wy = rotation.w * y;
	float wz = rotation.w * z;

	glm::mat4 matrix
	{
		1.0f - yy - zz,         xy + wz,         xz - wy,  0.0f,
		       xy - wz,  1.0f - xx - zz,         yz + wx,  0.0f,
		       xz + wy,         yz - wx,  1.0f - xx - yy,  0.0f,
		          0.0f,            0.0f,            0.0f,  1.0f
	};
	return matrix;
}

glm::mat4 Transform::scale_matrix()
{
	glm::mat4 matrix
	{
		scale.x,     0.0f,     0.0f,  0.0f,
		   0.0f,  scale.y,     0.0f,  0.0f,
		   0.0f,     0.0f,  scale.z,  0.0f,
		   0.0f,     0.0f,     0.0f,  1.0f
	};
	return matrix;
}

glm::mat4 Transform::matrix()
{
	//glm::translate();
	glm::mat4 pos = position_matrix();
	//glm::rotate();
	glm::mat4 rot = rotation_matrix();
	//glm::scale();
	glm::mat4 sca = scale_matrix();

	return sca /** rot*/ * pos;
}
