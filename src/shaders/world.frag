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
	mat4 projection;
	mat4 view;
	mat4 invView;
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
	vec3 specularLight = vec3(0.0);
	vec3 surfaceNormal = normalize(fragNormalWorld);

	vec3 cameraPosWorld = ubo.invViewp[3].xyz;
	vec3 viewDirection = normalize(cameraPosWorld - fragPosWorld);

	for (int i = 0; i < ubo.numLights; i++) {
		PointLight light = ubo.pointLights[i];
		vec3 vectorToLight = light.position.xyz - fragPosWorld;
		vec3 directionToLight = normalize(vectorToLight);

		float distanceAttenuation = 1.0 / dot(vectorToLight, vectorToLight); // distance squared

		// diffuse lighting
		float cosRefraction = max(dot(surfaceNormal, directionToLight), 0);
		vec3 lightIntensity = light.color.xyz * light.color.w * distanceAttenuation;
		diffuseLight += lightIntensity * cosRefraction;

		// specular lighting
		vec3 halfAngle = normalize(directionToLight + viewDirection);
		float blinnTerm = dot(surfaceNormal, halfAngle);
		blinnTerm = clamp(blinnTerm, 0, 1);
		blinnTerm = pow(blinnTerm, 512.0); // sharpness
		specularLight += lightIntensity * blinnTerm;
	}
	
	outColor = vec4(diffuseLight * fragColor + specularLight * fragColor, 1.0);
}