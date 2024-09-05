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
	mat4 modelMatrix;
} push;

const vec3 DIRECTION_TO_LIGHT = normalize(vec3(1.0, -3.0, -1.0));

// Output
// layout(location = 0) out vec3 fragColor; // use it if you need to test color blending

void main() {
	// Not B*A, use A*B for matrix multiplication
	//gl_Position = vec4(push.transform * position + push.offset, 1.0);
	//fragColor = color; // color is a special field for baricentric color blending

	gl_Position = push.transform * vec4(position, 1.0);

	// convert mat4 to mat3 remove translation, stayed only direction, which needed for
	// calculating object space normal to world space normal
	vec3 normalWorldSpace = normalize(mat3(push.modelMatrix) * normal);
	// Second variant
	//vec3 normalWorldSpace = normalize((push.modelMatrix * vec4(normal, 0.0)).xyz);

	float lightIntensity = max(dot(normalWorldSpace, DIRECTION_TO_LIGHT), 0);

	fragColor = lightIntensity * color;
}