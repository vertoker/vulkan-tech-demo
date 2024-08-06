#include "gameobject.h"

// position * rotation.y * rotation.x * rotation.z * scale
glm::mat4 Transform::matrix()
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
