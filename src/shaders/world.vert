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
layout(location = 1) out vec3 fragPosWorld;
layout(location = 2) out vec3 fragNormalWorld;

struct PointLight {
	vec4 position; // ignore w
	vec4 color; // w is intensity
};
layout(set = 0, binding = 0) uniform UniformBufferObject {
	mat4 projection;
	mat4 view;
	mat4 invView;
	vec4 ambientLightColor; // w is for intensity
	PointLight pointLights[10];
	int numLights;
} ubo;

// Input constants (individual data per drawing)
layout(push_constant) uniform Push {
// only 128 bit = 2 4x4 matrices
	mat4 modelMatrix;
	mat4 normalMatrix;
} push;

void main() {
	// Not B*A, use A*B for matrix multiplication
	//gl_Position = vec4(push.modelMatrix * position + push.offset, 1.0);
	//fragColor = color; // color is a special field for baricentric color blending

	vec4 positionWorld = push.modelMatrix * vec4(position, 1.0);
	gl_Position = ubo.projection * ubo.view * positionWorld;

	fragNormalWorld = normalize(mat3(push.normalMatrix) * normal);
	fragPosWorld = positionWorld.xyz;
	fragColor = color;
}