// GLSL fragment shader
#version 450

// Input
layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPosWorld;
layout(location = 2) in vec3 fragNormalWorld;

// Output
layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform UniformBufferObject {
	mat4 projectionMatrix;
	mat4 viewMatrix;

	vec4 ambientLightColor; // w is for intensity
	vec3 lightPosition;
	vec4 lightColor; // w is for intensity
} ubo;

layout(push_constant) uniform Push {
	mat4 modelMatrix;
	mat4 normalMatrix;
} push;

// Input location and Output location is different buffers

void main() {
	vec3 distanceToLight = ubo.lightPosition - fragPosWorld;
	vec3 directionToLight = normalize(distanceToLight);
	
	vec3 ambientLightColor = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;

	// dot product of vec3 itself is easy squared distance
	float attenuation = 1.0 / dot(distanceToLight, distanceToLight);
	vec3 lightColor = ubo.lightColor.xyz * ubo.lightColor.w * attenuation;
	vec3 diffuseLight = lightColor * max(dot(normalize(fragNormalWorld), directionToLight), 0);

	outColor = vec4((diffuseLight + ambientLightColor) * fragColor, 1.0);
}