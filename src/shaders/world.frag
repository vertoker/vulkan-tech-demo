// GLSL fragment shader
#version 450

// Input
layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPosWorld;
layout(location = 2) in vec3 fragNormalWorld;

// Output
layout(location = 0) out vec4 outColor;

struct PointLight {
	vec4 position; // ignore w
	vec4 color; // w is intensity
};
layout(set = 0, binding = 0) uniform UniformBufferObject {
	mat4 projectionMatrix;
	mat4 viewMatrix;
	vec4 ambientLightColor; // w is for intensity
	PointLight pointLights[10];
	int numLights;
} ubo;

layout(push_constant) uniform Push {
	mat4 modelMatrix;
	mat4 normalMatrix;
} push;

// Input location and Output location is different buffers

void main() {
	vec3 diffuseLight = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;
	vec3 surfaceNormal = normalize(fragNormalWorld);

	for (int i = 0; i < ubo.numLights; i++) {
		PointLight light = ubo.pointLights[i];
		vec3 vectorToLight = light.position.xyz - fragPosWorld;
		vec3 directionToLight = normalize(vectorToLight);

		float distanceAttenuation = 1.0 / dot(vectorToLight, vectorToLight); // distance squared
		float cosRefraction = max(dot(surfaceNormal, directionToLight), 0);

		vec3 lightIntensity = light.color.xyz * light.color.w * distanceAttenuation;
		diffuseLight += lightIntensity * cosRefraction;
	}
	
	outColor = vec4(diffuseLight * fragColor, 1.0);
}