#version 450

layout(location = 0) in vec2 fragOffset;
layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform UniformBufferObject {
	mat4 projectionMatrix;
	mat4 viewMatrix;

	vec4 ambientLightColor; // w is for intensity
	vec3 lightPosition;
	vec4 lightColor; // w is for intensity
} ubo;

void main() {
	outColor = ubo.lightColor;
}