// GLSL vertex shader
#version 450

//#extension GL_KHR_vulkan_glsl : enable
// (-1 -> 1 ; -1 -> 1)

// Input
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

layout(location = 0) out vec3 fragColor;

// Input constants (individual data per drawing)
layout(push_constant) uniform Push {
// only 128 bit = 2 4x4 matrices
	mat4 transform; // projection * view * model
	mat4 normalMatrix;
} push;

const vec3 DIRECTION_TO_LIGHT = normalize(vec3(1.0, -3.0, -1.0));
const float AMBIENT = 0.02;

// Output
// layout(location = 0) out vec3 fragColor; // use it if you need to test color blending

void main() {
	// Not B*A, use A*B for matrix multiplication
	//gl_Position = vec4(push.transform * position + push.offset, 1.0);
	//fragColor = color; // color is a special field for baricentric color blending

	gl_Position = push.transform * vec4(position, 1.0);

	// convert mat4 to mat3 remove translation, stayed only direction, which needed for
	// calculating object space normal to world space normal
	// works only if sx == sy == sz
	//vec3 normalWorldSpace = normalize(mat3(push.modelMatrix) * normal);

	//mat3 normalMatrix = transpose(inverse(mat3(push.modelMatrix)));
	//vec3 normalWorldSpace = normalize(normalMatrix * normal);

	vec3 normalWorldSpace = normalize(mat3(push.normalMatrix) * normal);

	float lightIntensity = AMBIENT + max(dot(normalWorldSpace, DIRECTION_TO_LIGHT), 0);

	fragColor = lightIntensity * color;
}