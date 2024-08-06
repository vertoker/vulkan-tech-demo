#include "gameobject.h"
#include <glm/gtc/quaternion.hpp>

/*glm::mat4 Transform::rotation_matrix() const
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

    glm::mat4 matrix {
        {
            1.0f - yy - zz,
            xy - wz,
            xz + wy,
            0.0f,
        },
        {
            xy + wz,
            1.0f - xx - zz,
            yz - wx,
            0.0f,
        },
        {
            xz - wy,
            yz + wx,
            1.0f - xx - yy,
            0.0f,
        },
        {
            0.0f,
            0.0f,
            0.0f,
            1.0f,
        }
    };
    return matrix;
}*/

/*glm::mat4 Transform::matrix()
{
    auto matrix = glm::translate(glm::mat4{ 1.0f }, position);
    matrix = matrix * rotation_matrix();
    matrix = glm::scale(matrix, scale);
    return matrix;
}*/

// position * rotation.y * rotation.x * rotation.z * scale
glm::mat4 Transform::matrix2()
{
	const float s1 = glm::sin(rotation.y);
	const float c1 = glm::cos(rotation.y);
	const float s2 = glm::sin(rotation.x);
	const float c2 = glm::cos(rotation.x);
	const float s3 = glm::sin(rotation.z);
	const float c3 = glm::cos(rotation.z);

    // vec4 is columns of floats
    // mat4 is row of vec4
    return glm::mat4 {
        {
            scale.x * (c1 * c3 + s1 * s2 * s3),
            scale.x * (c2 * s3),
            scale.x * (c1 * s2 * s3 - c3 * s1),
            0.0f,
        },
        {
            scale.y * (c3 * s1 * s2 - c1 * s3),
            scale.y * (c2 * c3),
            scale.y * (c1 * c3 * s2 + s1 * s3),
            0.0f,
        },
        {
            scale.z * (c2 * s1),
            scale.z * (-s2),
            scale.z * (c1 * c2),
            0.0f,
        },
        {
            position.x, 
            position.y,
            position.z, 
            1.0f
        } 
    };
}
